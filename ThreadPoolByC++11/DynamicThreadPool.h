#include"SyncQueue.h"

#include<thread>
#include<future>
#include <functional>
#include<chrono>
#include<map>

#ifndef __DYNAMIC_THREAD_POOL__
#define __DYNAMIC_THREAD_POOL__


namespace self_thread_pool
{
	class DynamicThreadPool
	{
	public:
		using Task = std::function<void(void)>;
		//创建对象即创建线程池，立即开始工作
		DynamicThreadPool(int task_num,int min_num) :
			m_task_pool(task_num), m_thread_min_num(min_num),m_thread_max_num(std::thread::hardware_concurrency()), m_keep_run(true), m_sleep_time(100)
		{
			for (int i = 0; i < min_num; ++i)
			{
				auto tha = std::make_shared<std::thread>(
					std::thread(&DynamicThreadPool::ThreadEntrance, this));
				
				std::thread::id tid = tha->get_id();
				m_threads.emplace(tid, std::move(tha));
			}
		}
		//析构函数，希望在此处能够把任务队列销毁，并等待所有线程结束
		~DynamicThreadPool()
		{
			Close();

		}
		//可以手工调用关闭函数，也可以等线程池自动消亡
		void Close()
		{
			std::call_once(m_called, [this]()->void {Close(1); });
		}

		//添加任务的函数，任务可能有返回值
		template<typename FUNC, typename ...ARGS>
		auto Submit(FUNC&& func, ARGS&&... arg)
		{
			//推断该函数的返回值类型
			using RetType = decltype(func(arg ...));
			//将该任务通过packaged_task进行打包，生成RetType(void)的函数类型
			auto task = std::make_shared<std::packaged_task<RetType(void)> >(
				std::bind(std::forward<FUNC>(func), std::forward<ARGS>(arg)...)
			);
			//异步获取该任务的返回值
			std::future<RetType> ret = task->get_future();
			//由于任务队列的函数对象应该是std::function<void(void)>,因此task任务无法直接添加至任务队列
			//故，利用lambda表达式调用task。
			
			//如果提交任务成功，那么无需创建新的线程
			if (m_task_pool.Submit([this, task](void)->void {(*task)(); }))
			{
				return ret;
			}
			//否则，将该任务由调用者线程完成，并创建新的线程，添加至线程组中
			else
			{
				(*task)();
				std::unique_lock<std::mutex> self_lock(m_mtx);
				if (m_threads.size() < m_thread_max_num)
				{
					//LOG(NEW THREAD);
					auto tha = std::make_shared<std::thread>(std::thread(&DynamicThreadPool::ThreadEntrance, this));
					auto tid = tha->get_id();
					m_threads.emplace(tid, std::move(tha));
				}
				self_lock.unlock();
				return ret;
			}
		}
	private:
		//线程的工作入口函数
		void ThreadEntrance()
		{
			
			while (m_keep_run)
			{
				//std::this_thread::sleep_for(std::chrono::seconds(1));
				Task task;
				if (m_task_pool.GetOneTask(task))
				{
					std::unique_lock<std::mutex> self_lock(m_mtx);
					task();
				}
				else
				{
					//如果取出任务失败，表明当前任务数量较少，可适当减少工作线程数（假销毁）
					//LOG(SLEEP THREAD);
					std::this_thread::sleep_for(std::chrono::milliseconds(m_sleep_time));
				}
			}
		}
		//线程池关闭函数
		void Close(int in)
		{
			m_task_pool.Close();
			m_keep_run = false;
			for (auto& thd : m_threads)
			{
				if (thd.second && thd.second->joinable())
				{
					thd.second->join();
				}
			}
			m_threads.clear();
		}
		DynamicThreadPool() = delete;
		DynamicThreadPool(const DynamicThreadPool& src) = delete;
		DynamicThreadPool(const DynamicThreadPool&& src) = delete;
		DynamicThreadPool& operator= (const DynamicThreadPool& src) = delete;
	private:
		//线程池是否保持运行
		std::atomic_bool m_keep_run;
		//线程池的任务队列
		SyncQueue<Task> m_task_pool;

		//在调用close(int)函数时，只允许调用一次
		std::once_flag m_called;
		//线程的数量范围
		int m_thread_min_num;
		int m_thread_max_num;
		//线程睡眠时间
		int m_sleep_time;
		//初始化后的线程组，利用智能指针存储在map中，以便停止当前线程，或者创建新的线程
		std::unordered_map<std::thread::id,std::shared_ptr<std::thread> > m_threads;
		//在创建新的线程时需要对线程组进行锁操作
		std::mutex m_mtx;
	};
}

#endif //__DYNAMIC_THREAD_POOL__

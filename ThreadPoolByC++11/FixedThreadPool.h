#include"SyncQueue.h"
#include<thread>
#include<future>
#include <functional>
#include<chrono>

#ifndef __FIX_THREAD_POOL__
#define __FIX_THREAD_POOL__

namespace self_thread_pool
{
	class FixedThreadPool
	{
	public:
		using Task = std::function<void(void)>;
		//创建对象即创建线程池，立即开始工作
		FixedThreadPool(int task_num, int thread_num):
			m_task_pool(task_num),m_thread_num(thread_num),m_keep_run(true)
		{
			for (int i = 0; i < thread_num; ++i)
			{
				m_threads.emplace_back(std::make_shared<std::thread>(&FixedThreadPool::ThreadEntrance,this));
			}
		}
		//析构函数，希望在此处能够把任务队列销毁，并等待所有线程结束
		~FixedThreadPool()
		{
			Close();
			
		}
		//可以手工调用关闭函数，也可以等线程池自动消亡
		void Close()
		{
			std::call_once(m_called, [this]()->void {Close(1); });
		}

		//添加任务的函数，任务可能有返回值
		template<typename FUNC,typename ...ARGS>
		auto Submit(FUNC && func, ARGS&&... arg)
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
			if (m_task_pool.Submit([this,task](void)->void {(*task)(); }))
			{
				return ret;
			}
			else
			{
				(*task)();
				return ret;
			}
		}
	private:
		//线程的工作入口函数
		void ThreadEntrance()
		{
			while (m_keep_run)
			{
				Task task;
				if (m_task_pool.GetOneTask(task))
				{
					std::unique_lock<std::mutex> self_lock(m_mtx);
					task();
				}
				else//这里暂时没必要
				{
					//好像没有啥需求，没任务就再次循环就好
					//还是睡眠一会儿吧
					//std::this_thread::sleep_for(std::chrono::seconds(1));
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
				if (thd && thd -> joinable())
				{
					thd -> join();
				}
			}
			m_threads.clear();
		}
		FixedThreadPool() = delete;
		FixedThreadPool(const FixedThreadPool& src) = delete;
		FixedThreadPool(const FixedThreadPool&& src) = delete;
		FixedThreadPool& operator= (const FixedThreadPool& src) = delete;
	private:
		//线程池是否保持运行
		std::atomic_bool m_keep_run;
		//线程池的任务队列
		SyncQueue<Task> m_task_pool;
		//在调用close(int)函数时，只允许调用一次
		std::once_flag m_called;
		//线程的数量
		int m_thread_num;
		//初始化后的线程组，利用智能指针存储
		std::vector<std::shared_ptr<std::thread> > m_threads;

		//该互斥锁无实际意义，只是为了更清楚地观察打印的信息
		std::mutex m_mtx;
	};
}
#endif //__FIX_THREAD_POOL__
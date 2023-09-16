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
		//�������󼴴����̳߳أ�������ʼ����
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
		//����������ϣ���ڴ˴��ܹ�������������٣����ȴ������߳̽���
		~DynamicThreadPool()
		{
			Close();

		}
		//�����ֹ����ùرպ�����Ҳ���Ե��̳߳��Զ�����
		void Close()
		{
			std::call_once(m_called, [this]()->void {Close(1); });
		}

		//�������ĺ�������������з���ֵ
		template<typename FUNC, typename ...ARGS>
		auto Submit(FUNC&& func, ARGS&&... arg)
		{
			//�ƶϸú����ķ���ֵ����
			using RetType = decltype(func(arg ...));
			//��������ͨ��packaged_task���д��������RetType(void)�ĺ�������
			auto task = std::make_shared<std::packaged_task<RetType(void)> >(
				std::bind(std::forward<FUNC>(func), std::forward<ARGS>(arg)...)
			);
			//�첽��ȡ������ķ���ֵ
			std::future<RetType> ret = task->get_future();
			//����������еĺ�������Ӧ����std::function<void(void)>,���task�����޷�ֱ��������������
			//�ʣ�����lambda���ʽ����task��
			
			//����ύ����ɹ�����ô���贴���µ��߳�
			if (m_task_pool.Submit([this, task](void)->void {(*task)(); }))
			{
				return ret;
			}
			//���򣬽��������ɵ������߳���ɣ��������µ��̣߳�������߳�����
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
		//�̵߳Ĺ�����ں���
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
					//���ȡ������ʧ�ܣ�������ǰ�����������٣����ʵ����ٹ����߳����������٣�
					//LOG(SLEEP THREAD);
					std::this_thread::sleep_for(std::chrono::milliseconds(m_sleep_time));
				}
			}
		}
		//�̳߳عرպ���
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
		//�̳߳��Ƿ񱣳�����
		std::atomic_bool m_keep_run;
		//�̳߳ص��������
		SyncQueue<Task> m_task_pool;

		//�ڵ���close(int)����ʱ��ֻ�������һ��
		std::once_flag m_called;
		//�̵߳�������Χ
		int m_thread_min_num;
		int m_thread_max_num;
		//�߳�˯��ʱ��
		int m_sleep_time;
		//��ʼ������߳��飬��������ָ��洢��map�У��Ա�ֹͣ��ǰ�̣߳����ߴ����µ��߳�
		std::unordered_map<std::thread::id,std::shared_ptr<std::thread> > m_threads;
		//�ڴ����µ��߳�ʱ��Ҫ���߳������������
		std::mutex m_mtx;
	};
}

#endif //__DYNAMIC_THREAD_POOL__

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
		//�������󼴴����̳߳أ�������ʼ����
		FixedThreadPool(int task_num, int thread_num):
			m_task_pool(task_num),m_thread_num(thread_num),m_keep_run(true)
		{
			for (int i = 0; i < thread_num; ++i)
			{
				m_threads.emplace_back(std::make_shared<std::thread>(&FixedThreadPool::ThreadEntrance,this));
			}
		}
		//����������ϣ���ڴ˴��ܹ�������������٣����ȴ������߳̽���
		~FixedThreadPool()
		{
			Close();
			
		}
		//�����ֹ����ùرպ�����Ҳ���Ե��̳߳��Զ�����
		void Close()
		{
			std::call_once(m_called, [this]()->void {Close(1); });
		}

		//�������ĺ�������������з���ֵ
		template<typename FUNC,typename ...ARGS>
		auto Submit(FUNC && func, ARGS&&... arg)
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
		//�̵߳Ĺ�����ں���
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
				else//������ʱû��Ҫ
				{
					//����û��ɶ����û������ٴ�ѭ���ͺ�
					//����˯��һ�����
					//std::this_thread::sleep_for(std::chrono::seconds(1));
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
		//�̳߳��Ƿ񱣳�����
		std::atomic_bool m_keep_run;
		//�̳߳ص��������
		SyncQueue<Task> m_task_pool;
		//�ڵ���close(int)����ʱ��ֻ�������һ��
		std::once_flag m_called;
		//�̵߳�����
		int m_thread_num;
		//��ʼ������߳��飬��������ָ��洢
		std::vector<std::shared_ptr<std::thread> > m_threads;

		//�û�������ʵ�����壬ֻ��Ϊ�˸�����ع۲��ӡ����Ϣ
		std::mutex m_mtx;
	};
}
#endif //__FIX_THREAD_POOL__
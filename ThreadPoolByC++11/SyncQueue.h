#include<iostream>
#include<chrono>
#include<list>
#include<mutex>
#include<condition_variable>

#ifndef __SYNC_QUEUE__
#define __SYNC_QUEUE__
#define LOG(X) std::cout<<#X<<std::endl;

namespace self_thread_pool
{
	template<typename T>
	class SyncQueue
	{
	public:
		//TODO
		SyncQueue(int max_tasks_nums = 16)
			:m_keep_run(true),
			m_max_size(max_tasks_nums),
			m_max_wait_time(2)
		{}
		//TODO
		~SyncQueue()
		{}
		//����������ύ����
		bool Submit(const T& task)
		{
			//LOG(T&);
			return AddTask(task);
		}
		bool Submit(T&& task)
		{
			//LOG(T&&);
			return AddTask(task);
		}
		//�����������ȡ������
		bool GetOneTask(T& task)
		{
			std::unique_lock<std::mutex> self_lock(m_mtx);
			bool success = m_allow_take.wait_for(self_lock,
				std::chrono::milliseconds(m_max_wait_time),
				[this]()->bool {return (!m_keep_run) || (m_keep_run && !IsEmpty()); });//����̳߳���Ҫֹͣ���������е��ǿգ���Ӧ�ý���wait����
			
			if (success)
			{//��ʱ�Ŀ��������1.�̳߳�ֹͣ����		2.�������в���������зǿ�
				if (m_keep_run)//2
				{
					task = m_tasks.front();
					m_tasks.pop_front();
					m_allow_submit.notify_one();
					return true;
				}
				else//1
				{
					return false;
				}
			}
			else//�ȴ�ʱ�䵽�����ȴ�ʱ�䣬���޷�ȡ������
			{
				return false;
			}
			//if (m_keep_run)
			//{
			//	task = m_tasks.front();
			//	m_tasks.pop_front();
			//	m_allow_submit.notify_one();
			//	return true;
			//}
			//else
			//{
			//	return false;
				//���ȡ����Ӧ����ʱ�����ƣ����������ȡ��ִ�У�û�о͵ȴ��ͺ�
				//bool success = m_allow_take.wait(self_lock,
				//	std::chrono::seconds(m_max_wait_time),
				//	[this]()->bool {return m_keep_run && !IsEmpty(); });
				//if(success)
				//{
				//	task = m_tasks.front();
				//	m_tasks.pop_front();
				//	m_allow_submit.notify_one();
				//	return true;
				//}
				//else
				//{
				//	task = [](void)->void {};
				//	return false;
				//}

			//}
		}
		//ֹͣ�����������
		void Close()
		{
			//LOG(CLOSE);
			std::unique_lock<std::mutex> self_lock(m_mtx);
			//�ȴ���������ִ�����
			while (m_keep_run && !IsEmpty())
			{
				//LOG(WHILE);
				m_allow_submit.wait(self_lock);
			}
			//�ر�����
			//LOG(CLOSE TRUE);
			m_keep_run = false;
			//�������еȴ��̣߳���ʹ���������
			m_allow_take.notify_all();
			m_allow_submit.notify_all();
		}
	private:
		bool IsEmpty() const
		{
			return m_tasks.empty();
		}
		bool IsFull() const
		{
			return m_tasks.size() == m_max_size;
		}
		//���������
		template<typename F>
		bool AddTask(F&& func)
		{
			std::unique_lock<std::mutex> self_lock(m_mtx);
			//�ȴ����ʵ�ʱ�������������������޶�ʱ���ڵõ���������ӵ�����������ӳɹ�
			bool success = m_allow_submit.wait_for(self_lock,
				std::chrono::seconds(m_max_wait_time),
				[this]()->bool {return (m_keep_run && !IsFull()); });
			//���������������
			if (success)
			{
				//LOG(SUB OK);
				m_tasks.push_back(std::forward<F>(func));
				m_allow_take.notify_one();
				return true;
			}
			//����޷���ӳɹ�
			else
			{
				//LOG(SUB FALSE);
				return false;
			}
		}
	private:
		//�����Ƿ񱣳�����
		bool m_keep_run;
		//������е���󳤶�
		int m_max_size;
		//�������
		std::list<T> m_tasks;
		//�����߳�ͬ���Ļ�����
		std::mutex m_mtx;
		//�������������ź���
		std::condition_variable m_allow_submit;
		//�����ȡ������ź���
		std::condition_variable m_allow_take;
		//�����������ȴ�ʱ��
		int m_max_wait_time;
	};
}
#endif //!__SYNC_QUEUE__
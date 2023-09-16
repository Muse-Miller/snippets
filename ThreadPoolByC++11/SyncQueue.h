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
		//向任务队列提交任务
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
		//从任务队列中取出任务
		bool GetOneTask(T& task)
		{
			std::unique_lock<std::mutex> self_lock(m_mtx);
			bool success = m_allow_take.wait_for(self_lock,
				std::chrono::milliseconds(m_max_wait_time),
				[this]()->bool {return (!m_keep_run) || (m_keep_run && !IsEmpty()); });//如果线程池需要停止、继续运行但非空，都应该结束wait函数
			
			if (success)
			{//此时的可能情况：1.线程池停止运行		2.保持运行并且任务队列非空
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
			else//等待时间到达最大等待时间，仍无法取得任务
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
				//这个取任务不应该有时间限制，有了任务就取出执行，没有就等待就好
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
		//停止运行任务队列
		void Close()
		{
			//LOG(CLOSE);
			std::unique_lock<std::mutex> self_lock(m_mtx);
			//等待所有任务执行完毕
			while (m_keep_run && !IsEmpty())
			{
				//LOG(WHILE);
				m_allow_submit.wait(self_lock);
			}
			//关闭运行
			//LOG(CLOSE TRUE);
			m_keep_run = false;
			//唤醒所有等待线程，来使其结束工作
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
		//添加任务函数
		template<typename F>
		bool AddTask(F&& func)
		{
			std::unique_lock<std::mutex> self_lock(m_mtx);
			//等待合适的时机进行添加任务，如果在限定时间内得到了允许添加的条件，则添加成功
			bool success = m_allow_submit.wait_for(self_lock,
				std::chrono::seconds(m_max_wait_time),
				[this]()->bool {return (m_keep_run && !IsFull()); });
			//如果条件可以满足
			if (success)
			{
				//LOG(SUB OK);
				m_tasks.push_back(std::forward<F>(func));
				m_allow_take.notify_one();
				return true;
			}
			//如果无法添加成功
			else
			{
				//LOG(SUB FALSE);
				return false;
			}
		}
	private:
		//队列是否保持运行
		bool m_keep_run;
		//任务队列的最大长度
		int m_max_size;
		//任务队列
		std::list<T> m_tasks;
		//进行线程同步的互斥锁
		std::mutex m_mtx;
		//允许添加任务的信号量
		std::condition_variable m_allow_submit;
		//允许读取任务的信号量
		std::condition_variable m_allow_take;
		//添加任务的最大等待时间
		int m_max_wait_time;
	};
}
#endif //!__SYNC_QUEUE__

#include"FixedThreadPool.h"
#include"DynamicThreadPool.h"
#include"SyncQueue.h"
using namespace self_thread_pool;

int Add(int a, int b)
{
	//std::cout << a + b << std::endl;
	//std::cout << std::this_thread::get_id() << ":\t";
	return a + b;
}
void Add1(int a, int b)
{
	std::cout << a + b << std::endl;
}
void Add2()
{
	std::cout << std::this_thread::get_id() << std::endl;
}
class Test
{
public:
	static int Add1(int a, int b)
	{
		return a + b;
	}
	int Add2(int a, int b)
	{
		return a + b;
	}
};
class Test2
{
public:
	int operator()(int a, int b)
	{
		return a + b;
	}
};


//同步队列功能测试
#if 0
int main()
{

	using Task = std::function<void()>;
	SyncQueue<Task> tql(2);
	
	tql.Submit(std::bind(Add1,1,2));
	tql.Submit(std::bind(Add1,2,3));
	
	Task task;
	
	tql.GetOneTask(task);
	task();

	tql.Submit(std::bind(Add1,3,4));
	
	tql.GetOneTask(task);
	task();
	
	tql.Submit(std::bind(Add1,4,5));
	
	tql.GetOneTask(task);
	task();
	
	tql.GetOneTask(task);
	task();

	std::thread thd(&SyncQueue<Task>::Close, &tql);

	tql.Submit(std::bind(Add1, 5, 6));

	tql.GetOneTask(task);
	task();
	tql.GetOneTask(task);
	task();
	tql.GetOneTask(task);
	task();
	thd.join();
	return 0;
}
#endif

//定长线程池功能测试
#if 0
int main()
{
	FixedThreadPool pool(4, 8);
	//std::thread t2([&pool]()->void
	//	{
	//		for (int i = 0; i < 10; ++i)
	//		{
	//			pool.Submit(Add2);
	//		}
	//	}
	//);
	//t2.join();
	//pool.Close();
	//std::cout << "T2 END" << std::endl;

	//for (int i = 0; i < 10; ++i)
	//{
	//	pool.Submit(Add2);
	//}
	//pool.Close();
	
	//for (int i = 0; i < 10; ++i)
	//{
	//	int ret = pool.Submit(Add, i, i).get();
	//	std::mutex mtx;
	//	std::unique_lock<std::mutex> lck(mtx);
	//	std::cout << ret << std::endl;
	//	lck.unlock();
	//}
	//pool.Close();

	Test2 a;
	for (int i = 0; i < 10; ++i)
	{
		int ret = pool.Submit(a,i,i).get();//std::bind(a,i,i)
		std::mutex mtx;
		std::unique_lock<std::mutex> lck(mtx);
		std::cout << ret << std::endl;
		lck.unlock();
	}
	std::thread t2([&pool,&a]()->void {
		for (int i = 0; i < 10; ++i)
		{
			int ret = pool.Submit(a,i,i).get();
			std::mutex mtx;
			std::unique_lock<std::mutex> lck(mtx);
			std::cout << ret << std::endl;
			lck.unlock();
		}});
	t2.join();
	pool.Close();
	std::cout << "T2 END" << std::endl;

	return 0;
}
#endif


//动态线程池功能测试
#if 0

void Producer(DynamicThreadPool& pool)
{
	for (int i = 0; i < 10; ++i)
	{
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		pool.Submit(Add1, i, i);
	}
}
int main()
{
	DynamicThreadPool pool(4, 8);

	std::thread t1(Producer,std::ref(pool));
	std::thread t2(Producer,std::ref(pool));
	//for (int i = 0; i < 10; ++i)
	//{
	//	pool.Submit(Add1, i, i);
	//}
	t1.join();
	t2.join();
	return 0;
}
#endif



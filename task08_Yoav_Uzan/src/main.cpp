#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;


struct Message
{
	thread::id tid;
	double value=0;
	bool finished=false;
};


mutex m;
condition_variable cv;
std::queue<Message> messages;

void producer()
{
	auto tid = this_thread::get_id();
	double value = hash<thread::id>{}(tid);
	for (; value > 0; value /= 10.0)
	{
		{
			unique_lock lock(m);
			messages.push({ tid, value, false });
			cv.notify_one();
		}
	}
	{
		unique_lock<mutex> lock(m);
		 messages.push({ tid, value, true });
		cv.notify_one();
	}
}

void consumer()
{
	 int count = 0;
	 bool flag = true;
	 Message msg;
	while (true)
	{
		{
			unique_lock lock(m);
			cv.wait(lock, []() {return !messages.empty(); });
			
			if (flag)
			{
				msg = messages.front();
				flag = false;
			}

			if (messages.front().finished)
			{
				cout << messages.front().tid  << " \033[1;32mFinsh\033[0m" << endl;
				messages.pop();
				count++;
			}
			else
			{
				if(msg.tid == messages.front().tid)
					cout << "\033[1;36m" << messages.front().tid << " sent- " << messages.front().value << "\033[0m\n";
				else
					cout <<  "\033[1;31m"<<messages.front().tid << " sent- " << messages.front().value << "\033[0m\n";
				messages.pop();
			}
			if (count == 2 && messages.empty())
				break;
		}
		
	}
}


int main()
{
try
{
	thread t3(consumer);
	thread t1(producer);
	thread t2(producer);
	

	t1.join();
	t2.join();
	t3.join();
}
catch (const exception& e)
{
	cout << e.what() << endl;
	return 1;
}
	return 0;
}

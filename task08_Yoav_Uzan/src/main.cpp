#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include "message.h"
using namespace std;

mutex m;
condition_variable cv;
std::queue<Message> messages;

void producer()
{
	auto tid = this_thread::get_id();
	double value = hash<thread::id>{}(tid);

	// Generate messages with decreasing values
	for (; value > 0; value /= 10.0)
	{
		{
			unique_lock lock(m);
			// Push the message into the queue
			messages.push({ tid, value, false });
			// Notify the consumer that a new message is available
			cv.notify_one();
		}
	}

	{
		unique_lock<mutex> lock(m);
		// Push a finished message into the queue
		messages.push({ tid, value, true });
		// Notify the consumer that a new message is available
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
			// Wait until there is a message available in the queue
			cv.wait(lock, []() { return !messages.empty(); });

			if (flag)
			{
				// Store the first message for comparison
				msg = messages.front();
				flag = false;
			}

			if (messages.front().finished)
			{
				// Print the finished message ID in green
				cout << messages.front().tid << " \033[1;32mFinsh\033[0m" << endl;
				messages.pop();
				count++;
			}
			else
			{
				if (msg.tid == messages.front().tid)
					// Print the message ID and value in cyan if it matches the first message
					cout << "\033[1;36m" << messages.front().tid << " sent- " << messages.front().value << "\033[0m\n";
				else
					// Print the message ID and value in red if it doesn't match the first message
					cout << "\033[1;31m" << messages.front().tid << " sent- " << messages.front().value << "\033[0m\n";
				messages.pop();
			}

			// Exit the loop if two finished messages have been processed and the queue is empty
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

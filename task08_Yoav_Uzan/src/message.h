#pragma once
#include <thread>
using namespace std;
struct Message
{
	thread::id tid;
	double value = 0;
	bool finished = false;
};
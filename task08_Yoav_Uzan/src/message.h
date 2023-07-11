#pragma once
#include <thread>
using namespace std;
struct Message
{
    thread::id tid;    // Thread ID associated with the message
    double value = 0;  // Value associated with the message
    bool finished = false;  // Flag indicating if the message is a finished message
};
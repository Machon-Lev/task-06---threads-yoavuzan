#pragma once
#include <thread>
using namespace std;
struct Message
{
    thread::id _tid;    // Thread ID associated with the message
    double _value = 0;  // Value associated with the message
    bool _finished = false;  // Flag indicating if the message is a finished message
};
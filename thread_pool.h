/**
  ******************************************************************************
  * @file           : thread_pool.h
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/21
  ******************************************************************************
  */



#ifndef THREAD_POOL_TOOLS_THREAD_POOL_H
#define THREAD_POOL_TOOLS_THREAD_POOL_H

#include "safe_queue.h"

#include <functional>

class ThreadPool {
private:

    SafeQueue<std::function<void()>> _tasks;

public:
    ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &&operator=(ThreadPool &&) = delete;

    
};


#endif //THREAD_POOL_TOOLS_THREAD_POOL_H

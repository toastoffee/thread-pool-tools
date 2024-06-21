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
#include <future>
#include <thread>

class ThreadPool {
public:

    std::vector<std::thread> _workers;
    SafeQueue<std::function<void()>> _tasks;
    std::condition_variable _cond;
    std::mutex _mtx;
    bool _shutdown;


public:
    explicit ThreadPool(int threads);
    ~ThreadPool();

    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;
    ThreadPool(ThreadPool &&) = delete;
    ThreadPool &&operator=(ThreadPool &&) = delete;

    template<typename F,typename... Args>
    auto AddTask(F &&f,Args &&...args) -> std::future<decltype(f(args...))>;

    static void WorkThread(ThreadPool *pool);

};


#endif //THREAD_POOL_TOOLS_THREAD_POOL_H

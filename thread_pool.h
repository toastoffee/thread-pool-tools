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

#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include <functional>

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

    template<class F,class... Args>
    auto AddTask(F &&f,Args &&...args) -> std::future<typename std::result_of<F(Args...)>::type>;

    static void WorkThread(ThreadPool *pool);

};


ThreadPool::ThreadPool(const int threads) : _shutdown(false) {
    for(int i = 0; i < threads; ++i){
        _workers.emplace_back(WorkThread, this);
    }
}

ThreadPool::~ThreadPool() {
    _shutdown = true;
    _cond.notify_all();

    for(auto &worker : _workers){
        worker.join();
    }
}

void ThreadPool::WorkThread(ThreadPool *pool) {
    while(true){
        std::function<void()> task;
        bool dequeued;

        {
            std::unique_lock<std::mutex> lck(pool->_mtx);

            pool->_cond.wait(lck, [pool]{ return pool->_shutdown || !pool->_tasks.Empty(); });

            if(pool->_shutdown && pool->_tasks.Empty()){
                return;
            }

            dequeued = pool->_tasks.Dequeue(task);
        }

        if(dequeued){
            task();
        }
    }
}

template<class F, class... Args>
auto ThreadPool::AddTask(F &&f, Args &&... args) ->  std::future<typename std::result_of<F(Args...)>::type> {

    using return_type = typename std::result_of<F(Args...)>::type;

    auto func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    auto taskPtr = std::make_shared<std::packaged_task<return_type()>>(func);

    std::future<return_type> res = taskPtr->get_future();

    std::function<void()> wrappedTask = [taskPtr](){
        (*taskPtr)();
    };

    if(_shutdown)
        throw std::runtime_error("enqueue on a shutdown threadPool");

    _tasks.Enqueue(wrappedTask);

    _cond.notify_one();

    return res;
}

#endif //THREAD_POOL_TOOLS_THREAD_POOL_H

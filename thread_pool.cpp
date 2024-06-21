/**
  ******************************************************************************
  * @file           : thread_pool.cpp
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/21
  ******************************************************************************
  */



#include "thread_pool.h"


ThreadPool::ThreadPool(const int threads) : _shutdown(false) {
    for(int i = 0; i < threads; ++i){
        _workers.emplace_back(WorkThread, this);
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

void ThreadPool::Shutdown() {

}

template<typename F, typename... Args>
auto ThreadPool::AddTask(F &&f, Args &&... args) -> std::future<decltype(f(args...))> {

    std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

    std::function<void()> wrappedTask = [taskPtr](){
        (*taskPtr)();
    };

    _tasks.Enqueue(wrappedTask);

    _cond.notify_one();

    return taskPtr->get_future();
}

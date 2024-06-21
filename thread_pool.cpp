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

ThreadPool::ThreadPool() {

}

template<typename F, typename... Args>
auto ThreadPool::AddTask(F &&f, Args &&... args) -> std::future<decltype(f(args...))> {

    std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

    auto taskPtr = std::make_shared<std::packaged_task<decltype(f(args...))()>>(func);

    std::function<void()> wrappedTask = [taskPtr](){
        (*taskPtr)();
    };

    _tasks.Enqueue(wrappedTask);

    return taskPtr->get_future();
}

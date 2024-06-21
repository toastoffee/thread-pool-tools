/**
  ******************************************************************************
  * @file           : safe_queue.h
  * @author         : toastoffee
  * @brief          : None
  * @attention      : None
  * @date           : 2024/6/21
  ******************************************************************************
  */



#ifndef THREAD_POOL_TOOLS_SAFE_QUEUE_H
#define THREAD_POOL_TOOLS_SAFE_QUEUE_H

#include <queue>
#include <mutex>

template<typename T>
class SafeQueue {
private:
    std::queue<T> _queue;
    std::mutex _mtx;

public:
    SafeQueue() = default;
    ~SafeQueue() = default;

    bool Empty();
    int Size();

    void Enqueue(T &t);
    bool Dequeue(T &t);

};

template<typename T>
bool SafeQueue<T>::Empty() {
    std::unique_lock<std::mutex> lck (_mtx);

    return  _queue.empty();
}

template<typename T>
int SafeQueue<T>::Size() {
    std::unique_lock<std::mutex> lck(_mtx);

    return _queue.size();
}

template<typename T>
void SafeQueue<T>::Enqueue(T &t) {
    std::unique_lock<std::mutex> lck(_mtx);

    _queue.emplace(&t);
}

template<typename T>
bool SafeQueue<T>::Dequeue(T &t) {
    std::unique_lock<std::mutex> lck(_mtx);

    if(_queue.empty()){
        return false;
    }else{
        t = std::move(_queue.front());
        _queue.pop();

        return true;
    }
}

#endif //THREAD_POOL_TOOLS_SAFE_QUEUE_H

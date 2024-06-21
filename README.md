# thread_pool_tools
a c++11 implementation of thread pool

### easy-usage

```c++
// 1. include the thread_pool file
#include "thread_pool.h"

// 2. initialize thread pool
int threadNum = 10;
ThreadPool pool(threadNum);

// 3. assign the task
std::future<int> res = pool.AddTask([]{ 
	int i = 2;
  return i * i;
})
  
// 4. get the result by std::future
int ans = res.get();
```



### auto-seal

```c++
// 1. use lambda function
for(int i = 0; i < 10; ++i) {
    results.emplace_back(
        pool.AddTask([i]{
          return i*i;
        })
    );
}

// 2. use declined function
for(int i = 0; i < 10; ++i) {
    results.emplace_back(
        pool.AddTask(square, i)
    );
}

```


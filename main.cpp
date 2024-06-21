#include <iostream>
#include <future>


int main ()
{

    std::future<int>  result = std::async([](){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return 8;
    });

    std::cout << "the future result : " << result.get() << std::endl;
    std::cout << "the future status : " << result.valid() << std::endl;

    return 0;
}

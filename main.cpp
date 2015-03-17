#include <iostream>
#include <thread>

void function1(void)
{
    std::cout << "hello world I'm function1" << std::endl; 

}

int main(int argc, char* argv[])
{
    int numCPUs = std::thread::hardware_concurrency();
    std::cout << "number of CPUs = " << numCPUs << std::endl;
    
    std::thread thread1(function1);
    
    //main thread detaches thread1 
    thread1.detach();
    
    //......

    if (thread1.joinable())
    { 
        thread1.join();
    }

    //demonstrate the power and syntax of lambda function in C++
    auto myByeFunction = [](std::string paramPrintMessage)->void {   std::cout << "BYE " << paramPrintMessage << std::endl;   };
    myByeFunction(" - this is end of the main thread");
    
    std::cout.flush();
    return 0;
} 

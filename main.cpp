#include <iostream>
#include <thread>

void function1(void)
{
    std::cout << "hello world I'm function1" << std::endl; 

}

int main(int argc, char* argv[])
{
    std::thread thread1(function1);
    
    //main thread waits for tread1 to finish
    thread1.join();
    
    //demonstrate the power and syntax of lambda function in C++
    []()->void {   std::cout << "BYE" << std::endl;   };
    
    std::cout.flush();
    return 0;
} 

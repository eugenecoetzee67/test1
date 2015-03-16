#include <iostream>
#include <thread>

void function1(void)
{
    std::cout << "hello world I'm function1" << std::endl; 

}

int main(int argc, char* argv[])
{
    std::thread thread1(function1);
    
    //main therad waits for tread1 to finish
    thread1.join();
    return 0;
} 

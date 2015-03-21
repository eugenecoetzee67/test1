#include <iostream>
#include <thread>

void function1(void)
{
    std::cout << "\nhello world I'm function1 in thread1 " << std::endl; 
    std::cout.flush();
}

int main(int argc, char* argv[])
{
    int numCPUs = std::thread::hardware_concurrency();
    std::cout << "\nnumber of CPUs = " << numCPUs << std::endl;
    
    std::thread thread1(function1);
    
    for (int i = 0;  i <= 100;  i ++)
    {
        std::cout << "MT = " << i << "."; 
    }     

    if (thread1.joinable() == true)
    {   
        std::cout << "\nthread1 joining main thread(MT) " << std::endl;
        thread1.join();
    }

    //demonstrate the power and syntax of lambda function in C++
    auto myByeFunction = [](std::string paramPrintMessage)->void {   std::cout << "\nBYE " << paramPrintMessage << std::endl;   };
    myByeFunction(" - this is end of the main thread");
    
    std::cout.flush();
    return 0;
} 

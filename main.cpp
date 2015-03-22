#include <iostream>
#include <thread>

void function1(void)
{
    std::cout << "\nhello world I'm function1 in thread1 " << std::endl; 
    std::cout.flush();
}

class Functor1
{
public:
    void operator()(std::string paramMessage)
    {
        std::cout << "hello im functor 1 saying " << paramMessage << std::endl;
     }	
	
};

int main(int argc, char* argv[])
{
    int numCPUs = std::thread::hardware_concurrency();
    std::cout << "\n number of CPUs = " << numCPUs << std::endl;
    std::cout << "\n main thread id = " << std::this_thread::get_id() << std::endl;
  
    std::thread thread1(function1);
    Functor1 myFunct1;
    std::thread thread2(myFunct1, "walk like a man");
    
    std::cout << "\n thread1 id = " << thread1.get_id() << std::endl;
    
    std::cout << "\n thread2 id = " << thread2.get_id() << std::endl;
    
    try
    {
        for (int i = 0;  i <= 100;  i ++)
        {
            std::cout << "MT = " << i << ".";
        }     
    } 
    catch(...)
    {
        thread1.join();
        throw;
    }
  

    if (thread1.joinable() == true)
    {   
        std::cout << "\nthread1 joining main thread(MT) " << std::endl;
        thread1.join();
    }

if (thread2.joinable() == true)
{
	thread2.join();
}

    //demonstrate the power and syntax of lambda function in C++
    auto myByeFunction = [](std::string paramPrintMessage)->void {   std::cout << "\nBYE " << paramPrintMessage << std::endl;   };
    myByeFunction(" - this is end of the main thread");
    
    std::cout.flush();
    return 0;
} 
#include <iostream>
#include <thread>

void function1(void)
{
    //std::cout << "\nhello world I'm function1 in ***THREAD1*** " << std::endl; 
    for (int i = 100;  i >= 0;  i --)
    {
        std::cout << "T1 = " << i << " * ";
    }     
}

class Functor1
{
public:
    void operator()(std::string& paramMessage)
    {
        std::cout << "hello im functor 1 %%%THREAD2%%% saying " << paramMessage << std::endl;
        paramMessage = "no no walk ... like a girl";
    }	
	
};

int main(int argc, char* argv[])
{   
    //clear console and move curses to top left corner
    std::cout << "\x1B[2J\x1B[H"; 
    int numCPUs = std::thread::hardware_concurrency();
    std::cout << "\n number of CPUs = " << numCPUs << std::endl;
    std::cout << "\n main thread id = " << std::this_thread::get_id() << std::endl;
 
    std::string message = "walk like a man";
    std::thread thread1(function1);
//    std::thread thread2((Functor1()), std::ref(message));
    
    std::cout << "\n thread1 id = " << thread1.get_id() << std::endl;
    
//    std::cout << "\n thread2 id = " << thread2.get_id() << std::endl;
    
    try
    {
        for (int i = 0;  i <= 100;  i ++)
        {
            std::cout << "MT = " << i << " ? ";
        }     
    } 
    catch(...)
    {
        thread1.join();
        throw;
    }
  

    if (thread1.joinable() == true)
    {   
        std::cout << "\nthread1 joining ???MAIN??? thread(MT) " << std::endl;
        thread1.join();
    }

//    if (thread2.joinable() == true)
//    {
//        std::cout << "\nthread2 joining ???MAIN??? thread(MT) " << std::endl;
//        thread2.join();
//    }
 
    std::cout << "\nthis is how message was changed by functor : " << message << std::endl;

    //demonstrate the power and syntax of lambda function in C++
    auto myByeFunction = [](std::string paramPrintMessage)->void {   std::cout << "\nbye " << paramPrintMessage << std::endl;   };
    myByeFunction(message);
    
    std::cout.flush();
    return 0;
} 

#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>

class LogFile
{
private:
    std::mutex mutex1; 
    std::ofstream outputFile; 

/* Notes:
 1) Never return outputFile to outside world
 2) Never pass outputFile as an argument to a function
 3) Design interfaces to consolodate access to data members
    in the same critical section 
*/

public:
    LogFile()
    { 
        outputFile.open("log.txt");

    }
    ~LogFile()
    {
        outputFile.close();

    }

    void sharedPrint(std::string paramMessage, int paramCounter, std::string paramThreadMarker)
    {
        //use RAII instead of raw mutex to prevent dead locks
        std::unique_lock<std::mutex> locker1(mutex1, std::defer_lock);
        //std::lock_guard<std::mutex> guard1(mutex1);
        //mutex1.lock();
        //outputFile << paramMessage << paramCounter << paramThreadMarker;
        
        locker1.lock();
        std::cout << paramMessage << paramCounter << paramThreadMarker;
        
        if (paramCounter == 50)
        {
            outputFile.flush();
            return;
        }
        locker1.unlock();
        
        //mutex1.unlock();
    } 

};


void function1(LogFile& paramLogFile)
{
    //std::cout << "\nhello world I'm function1 in ***THREAD1*** " << std::endl; 
    for (int i = 1000;  i >= 0;  i --)
    {
        paramLogFile.sharedPrint("T1 = ", i, " *********** ");
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
    LogFile myLogFile;
    //clear console and move curses to top left corner
    std::cout << "\x1B[2J\x1B[H"; 
    int numCPUs = std::thread::hardware_concurrency();
    std::cout << "\n number of CPUs = " << numCPUs << std::endl;
    std::cout << "\n main thread id = " << std::this_thread::get_id() << std::endl;
 
    std::string message = "walk like a man";
    std::thread thread1(function1, std::ref(myLogFile));
//    std::thread thread2((Functor1()), std::ref(message));
    
    std::cout << "\n thread1 id = " << thread1.get_id() << std::endl;

    
//    std::cout << "\n thread2 id = " << thread2.get_id() << std::endl;
    try
    {
        for (int i = 0;  i <= 1000;  i ++)
        {
            myLogFile.sharedPrint("MT = ", i, " ??????????? ");
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
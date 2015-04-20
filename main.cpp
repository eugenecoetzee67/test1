#include <iostream>
//#include <thread>
#include <future>

using namespace std;

int factorial(future<int>&  paramFutureN)
{
    int result = 1;
    cout << "factorial function executing in thread id = " << this_thread::get_id() << endl;
    cout << "I'm waiting for that promised value of N " << endl;
    int N = paramFutureN.get();
    cout << "I got that promised value of N now! " << endl;
    for (int i = N;  i > 1;  i--)
    {
        result *= i; 
    }
    
    return result; 
}

int main(int argc, char* argv[])
{   
    //clear console and move curses to top left corner
    cout << "\x1B[2J\x1B[H"; 
    int numCPUs = std::thread::hardware_concurrency();
    cout << "number of CPUs = " << numCPUs << endl;
    cout << "main thread id = " << this_thread::get_id() << endl;
 
    //thread thread1(factorial, 4);
    
    //future<int> future1 = async(factorial, 4);
    
    //doesn't compile on g++ 4.6 
    //future<int> future1 = async(launch::deferred, factorial, 4);
   
    promise<int> promise1;
    future<int> futureArg = promise1.get_future(); 
    future<int> future1 = async(launch::async, factorial, ref(futureArg));
     
    //sleep a bit 
    this_thread::sleep_for(chrono::seconds(2));    

    promise1.set_value(4);

    //force exception if the promise cannot be kept
    //promise1.set_exception(make_exception_ptr(runtime_error("to err is human")));
    
    int result1 = future1.get();

    cout << "result1 = " << result1 << endl; 

    //thread1.join();    

    return 0;
} 

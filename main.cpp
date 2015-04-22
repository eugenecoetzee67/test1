#include <iostream>
#include <thread>
#include <future>

using namespace std;

class Example
{

public:
    
    int operator()(int paramN, string paramDescription)
    {
        int result = 1;
        cout << "this thread id = " << this_thread::get_id()<< " called with descriptor " << paramDescription << endl;
        for (int i = paramN;  i > 1;  i--)
        {
            result *= i; 
        }
    
        return result; 
    }
    
    double returnDouble(int paramNumber)
    {
        return (double) paramNumber; 
    }

};

int main(int argc, char* argv[])
{   
    //clear console and move curses to top left corner
    cout << "\x1B[2J\x1B[H"; 
    int numCPUs = thread::hardware_concurrency();
    cout << "number of CPUs = " << numCPUs << endl;
    cout << "main thread id = " << this_thread::get_id() << endl;
    Example myCallable1;
    
    //create a copy of myCallable1() and invoke as functor in a different thread
    thread thread1(myCallable1, 5, "thread object invoked with a copy of collable object");

    //launch myCallable1 as a functor in a diferent thread
    thread thread2(ref(myCallable1), 6, "thread object invoked with original callable object");


 
    async(launch::async, myCallable1, 10, "async function");
    bind(myCallable1, 12, "bind function");
    once_flag myOnceFlag1;
    call_once(myOnceFlag1, myCallable1, 15, "call_once function");

    thread1.join();
    thread2.join();
    return 0;
} 

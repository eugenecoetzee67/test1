#include <iostream>
#include <thread>
#include <future>

using namespace std;

class Example
{
private:
    int instanceId; 

public:
    static int instanceCount;
    Example()
    {  
       instanceCount++;
       instanceId = instanceCount;
       cout << "CONSTRUCTOR thread id = " <<  this_thread::get_id() << " instance id = " << instanceId << endl;  
    }
    
    ~Example()
    {
       cout << " destructor thread id = " <<  this_thread::get_id() << " instance id = " << instanceId << endl;  
    }
    int operator()(int paramN, string paramDescription)
    {
        int result = 1;
        cout << "this thread id = " << this_thread::get_id()<<" instance_id = " << instanceId << " called with descriptor " << paramDescription << endl;
        for (int i = paramN;  i > 1;  i--)
        {
            result *= i; 
        }
    
        return result; 
    }
    
    Example(const Example &paramExample)
    {
       instanceCount++;
       instanceId = instanceCount;
       cout << "COPY CONSTRUCTOR thread id = " <<  this_thread::get_id() << " instance id = " << instanceId << endl;  
    }   
 
    double returnDouble(int paramNumber, string paramDescription)
    {
        cout << "this thread id = " << this_thread::get_id()<<" instance_id = " << instanceId << " called with descriptor " << paramDescription << endl;
        return (double) paramNumber; 
    }

};

void Foo(int paramInt)
{
    cout << "normal function called from thread id = " << this_thread::get_id() << " calculated value = " << paramInt << endl;
}   

int Example::instanceCount = 0;

int main(int argc, char* argv[])
{   
    //clear console and move curses to top left corner
    cout << "\x1B[2J\x1B[H"; 
    int numCPUs = thread::hardware_concurrency();
    cout << "number of CPUs = " << numCPUs << endl;
    cout << "main thread id = " << this_thread::get_id() << endl;
        
    Example myCallable1;
    
    //create a copy of myCallable1 and invoke as functor in a different thread
    thread thread1(myCallable1, 5, "thread object invoked with a copy of collable object");

    //launch myCallable1 as a functor in a different thread
    thread thread2(ref(myCallable1), 6, "thread object invoked with original callable object");

    //create temporary myCallable object
    thread thread3(Example(), 8, "thread object invoked from temporay instantiation");

    //create thread with a lambda function
    thread thread4([&](int paramInt){ cout << "lambda function called from thread id = " << this_thread::get_id() << " calculated value = " << paramInt + numCPUs << endl;}, 6);  
    
    //create thread with function
    thread thread5(Foo, 9);

    //create a copy of myCallable1 and invoke thread with member function
    thread thread6(&Example::returnDouble, myCallable1, 45.3, "thread object invoked from class member function");
    
    //send over addres of  myCallable1 (don't create copy) and invoke thread with member function
    thread thread7(&Example::returnDouble, &myCallable1, 45.3, "thread object invoked from class member function");

     //move myCallable1 to thread that is being invoked
    thread thread8(move(myCallable1), 20, "thread object invoked with a moved object"); 

    async(launch::async, myCallable1, 10, "async function");
    bind(myCallable1, 12, "bind function");
    once_flag myOnceFlag1;
    call_once(myOnceFlag1, myCallable1, 15, "call_once function");

    thread1.join();
    thread2.join();
    thread3.join();
    thread4.join();
    thread5.join();
    thread6.join();
    thread7.join();
    thread8.join();
    
    return 0;
} 

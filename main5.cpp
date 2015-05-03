#include <iostream>
//#include <thread>
#include <future>

using namespace std;

int factorial(int N)
{
    int result = 1;
    cout << "factorial function executing in thread id = " << this_thread::get_id() << endl;
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
    
    packaged_task<int()> packagedFactorial(bind(factorial, 5));
    packagedFactorial();
    
    int result1 = packagedFactorial.get_future().get();

    cout << "result1 = " << result1 << endl; 

     unique_ptr<int> myIntPtr(new int(3));
     auto myIntPtr2 = move(myIntPtr);
     cout << myIntPtr2.get() << " = " << *myIntPtr2 << endl; 

    //thread1.join();    
    
    return 0;
} 

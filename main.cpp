#include <iostream>
//#include <thread>
#include <future>

using namespace std;

int factorial(int N)
{
    int result = 1;
    for (int i = N;  i > 1;  i--)
    {
        result *= i; 
    }
    
    cout << "factorial function executing in thread id = " << this_thread::get_id() << endl;
    //cout << "result = " << result << endl;
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
   
    future<int> future1 = async(launch::async, factorial, 4);
    
    int result = future1.get();

    cout << "result = " << result << endl; 

    //thread1.join();    

    return 0;
} 

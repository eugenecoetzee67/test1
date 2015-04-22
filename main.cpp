#include <iostream>
#include <thread>
#include <future>

using namespace std;

class Example
{

public:

    int operator()(int paramN)
    {
        int result = 1;
        cout << "this thread id = " << this_thread::get_id()<< endl;
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
    Example myExample1;
    thread thread1(myExample1, 5); 
    async(launch::async, myExample1, 10);

    thread1.join();
    return 0;
} 

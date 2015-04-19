#include <iostream>
#include <thread>

using namespace std;

void factorial(int N)
{
    int result = 1;
    for (int i = N;  i > 1;  i--)
    {
        result *= i; 
    }
    
    cout << "result = " << result << endl; 
}

int main(int argc, char* argv[])
{   
    //clear console and move curses to top left corner
    cout << "\x1B[2J\x1B[H"; 
    int numCPUs = std::thread::hardware_concurrency();
    cout << "number of CPUs = " << numCPUs << endl;
    cout << "main thread id = " << this_thread::get_id() << endl;
 
    thread thread1(factorial, 4);

    thread1.join();    

    return 0;
} 

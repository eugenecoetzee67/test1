#include<iostream>
#include<fstream>
#include<vector>
#include<stdexcept>
#include<future>
#include<chrono>

#include<unistd.h>

using namespace std;

constexpr char DIR_SLASH[] = "/";
constexpr int MAX_FILE_SIZE = 100000000;
constexpr char EXCEPT_EMPTY_STRING_ARGUMENT[] = "empty string argument error";
constexpr char EXCEPT_ILLEGAL_STRING_ARGUMENT[] = "illegal string argument error";
constexpr char EXCEPT_EMPTY_BUFFER_ARGUMENT[] = "empty buffer argument error";

static const string FILENAME = "adt-bundle-linux-x86-20131030.zip";
static const string SOURCE_FILE_PATH = "/home/eugene/Downloads";
string DESTINATION_PATH = "/tmp/error";

vector<char> readInputFile(const string& paramInputFileName)
{
    cout << "enter function readInputFile in thread no = " << this_thread::get_id() << endl; 
    if (paramInputFileName.empty())
    {
        throw invalid_argument(EXCEPT_EMPTY_STRING_ARGUMENT);
    }
    ifstream inputFile(paramInputFileName, ios::binary | ios::ate );
    if (!inputFile.is_open())
    {
        cout << "could not open file " << paramInputFileName << endl;
        throw invalid_argument(EXCEPT_ILLEGAL_STRING_ARGUMENT);
    }
    size_t fileSize = inputFile.tellg(); 
    cout << "input file size = " << fileSize << " bytes " << endl;
    vector<char> inputBuffer;
    try
    {
        inputBuffer.resize(fileSize);
    }
    catch(bad_alloc& allocationException)
    {
        cout << "could not allocate" << fileSize << " bytes " << endl;
        throw;
    }

    inputFile.seekg(0, ios::beg);
    inputFile.read(&inputBuffer[0], fileSize);
    cout << inputFile.gcount() << " bytes were read " << endl;
    inputFile.close();
    this_thread::sleep_for(chrono::seconds(5));  
    cout << "exit function readInputFile in thread no = " << this_thread::get_id() << endl; 
    return inputBuffer;

}

size_t writeOutputFile(const vector<char>& paramInputBuffer, const string& paramOutputFileName)
{
    cout << "enter function writeOutputFile in thread no = " << this_thread::get_id() << endl; 
    if (paramOutputFileName.empty())
    {
        throw invalid_argument(EXCEPT_EMPTY_STRING_ARGUMENT);
    }
    else if (paramInputBuffer.empty())
    {
        throw invalid_argument(EXCEPT_EMPTY_BUFFER_ARGUMENT);
    }
    ofstream outputFile(paramOutputFileName, ios::binary);
    if (!outputFile.is_open())
    {
        cout << "could not open file " << paramOutputFileName << endl;
        throw invalid_argument(EXCEPT_ILLEGAL_STRING_ARGUMENT);
    }
    outputFile.write(&paramInputBuffer[0], paramInputBuffer.size());
    size_t numberOfBytesWritten = outputFile.tellp();
    outputFile.close();
    cout << numberOfBytesWritten << " bytes were written " << endl;
    cout << "exit function writeOutputFile in thread no = " << this_thread::get_id() << endl; 
    return numberOfBytesWritten;
}     

size_t threadedSyncedReadWriteFile(const string& paramInputFileName, const string& paramOutputFileName)
{
   cout << "enter function threadedSyncedReadWriteFile in thread no = " << this_thread::get_id() << endl; 
   
   promise<vector<char>> promisedInputBuffer;
   future<vector<char>> futureReadInputFile = promisedInputBuffer.get_future();
   thread fileReadThread([&promisedInputBuffer, paramInputFileName](){promisedInputBuffer.set_value(readInputFile(paramInputFileName));}); 
   
   promise<size_t> promisedNumberOfBytesWritten;
   future<size_t> futureWriteOutputFile = promisedNumberOfBytesWritten.get_future();
   thread fileWriteThread([&futureReadInputFile, &promisedNumberOfBytesWritten, paramOutputFileName](){promisedNumberOfBytesWritten.set_value(writeOutputFile(futureReadInputFile.get(),paramOutputFileName));});   
   size_t numberOfBytesWritten = futureWriteOutputFile.get();
   fileReadThread.join();
   fileWriteThread.join();
   cout << "exit function threadedSyncedReadWriteFile in thread no = " << this_thread::get_id() << endl; 
   return numberOfBytesWritten;
   return 0;
}

size_t packagedSyncedReadWriteFile(const string& paramInputFileName, const string& paramOutputFileName)
{
   cout << "enter function packagedSyncedReadWriteFile in thread no = " << this_thread::get_id() << endl; 
   
   //note - this does not compile under gcc 4.6!!
   packaged_task<vector<char>(const string&)> packagedReadInputFile(readInputFile);
   future<vector<char>> futureReadInputFile = packagedReadInputFile.get_future();
   thread fileReadThread(move(packagedReadInputFile), paramInputFileName); 
   
   /*promise<size_t> promisedNumberOfBytesWritten;
   future<size_t> futureWriteOutputFile = promisedNumberOfBytesWritten.get_future();
   thread fileWriteThread([&futureReadInputFile, &promisedNumberOfBytesWritten, paramOutputFileName](){promisedNumberOfBytesWritten.set_value(writeOutputFile(futureReadInputFile.get(),paramOutputFileName));});   
   size_t numberOfBytesWritten = futureWriteOutputFile.get();
   fileReadThread.join();
   fileWriteThread.join();
   cout << "exit function packagedSyncedReadWriteFile in thread no = " << this_thread::get_id() << endl; 
   return numberOfBytesWritten;*/
   return 0;
}
int main(int argc, char* argv[])
{   cout << "\x1B[2J\x1B[H"; 
    cout << "main function in thread no = " << this_thread::get_id() << endl; 
    string sourceFileName = "/tmp/error/error.txt";
    string destinationFileName = "/tmp/error/error.txt";
    char* currentPath = get_current_dir_name();
    if (currentPath)
    {
        DESTINATION_PATH  = currentPath;
        free(currentPath);
        currentPath = nullptr;  
    }
    sourceFileName = SOURCE_FILE_PATH + DIR_SLASH + FILENAME;
    destinationFileName = DESTINATION_PATH + DIR_SLASH + FILENAME;
    cout << "source filename = " << sourceFileName << endl;
    cout << "destination filename = " << destinationFileName << endl;
    
    //method 1 - use promise and future
    //promise<size_t> promisedNumberOfBytesWritten;
    //future<size_t>  futureThreadedSyncedReadWriteFile = promisedNumberOfBytesWritten.get_future();
    //thread readWriteThread([&promisedNumberOfBytesWritten, sourceFileName, destinationFileName](){promisedNumberOfBytesWritten.set_value(threadedSyncedReadWriteFile(sourceFileName, destinationFileName));});

    //method two use async
    future<size_t> futureThreadedSyncedReadWriteFile = async(launch::async, packagedSyncedReadWriteFile, sourceFileName, destinationFileName); 

    cout << "main thread is doing some work " << endl;  
    for (int i = 0;  i <= 500000;  i++)
    {    
        cout << "\r" << i;
    }
    
    //readWriteThread.join();
    size_t numebrOfBytesWritten = futureThreadedSyncedReadWriteFile.get();    

    cout << "\nbye" << endl;

    return EXIT_SUCCESS;
}
#include<iostream>
#include<fstream>
#include<vector>
#include<stdexcept>
#include<thread>
#include<chrono>

#include<unistd.h>

using namespace std;

const string DIR_SLASH = "/";
const int MAX_FILE_SIZE = 100000000;
const string EXCEPT_EMPTY_STRING_ARGUMENT = "empty string argument error";
const string EXCEPT_ILLEGAL_STRING_ARGUMENT = "illegal string argument error";
const string EXCEPT_EMPTY_BUFFER_ARGUMENT = "empty buffer argument error";
const string FILENAME = "adt-bundle-linux-x86-20131030.zip";
const string SOURCE_FILE_PATH = "/home/eugene/Downloads";
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

size_t syncedReadWriteFile(const string& paramInputFileName, const string& paramOutputFileName)
{
   cout << "enter function syncedReadWriteFile in thread no = " << this_thread::get_id() << endl; 
   size_t numberOfBytesWritten = writeOutputFile(readInputFile(paramInputFileName), paramOutputFileName);
   cout << "exit function syncedReadWriteFile in thread no = " << this_thread::get_id() << endl; 
   return numberOfBytesWritten;
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
    
    int numberOfBytesWritten = syncedReadWriteFile(sourceFileName,destinationFileName);    

    cout << "main thread is doing some work " << endl;  
    for (int i = 0;  i <= 500000;  i++)
    {    
        cout << "\r" << i;
    }
    cout << "\nbye" << endl;
    return EXIT_SUCCESS;

}

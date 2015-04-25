#include<iostream>
#include<fstream>
#include<vector>
#include<stdexcept>

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
 
    vector<char> inputBuffer;
    inputFile.close();
    return inputBuffer;

}

size_t writeOutputFile(const vector<char>& paramInputBuffer, const string& paramOutputFileName)
{
    if (paramOutputFileName.empty())
    {
        throw invalid_argument(EXCEPT_EMPTY_STRING_ARGUMENT);
    }
    else if (paramInputBuffer.empty())
    {
        throw invalid_argument(EXCEPT_EMPTY_BUFFER_ARGUMENT);
    }

    return 0;
}     



int main(int argc, char* argv[])
{
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
    
    vector<char> inputBuffer = readInputFile(sourceFileName); 
    

    return EXIT_SUCCESS;

}

// END SEMESTER PROJECT BY:
// TAYYAB AHMED     CMS: 294827
// SAROSH KRISHAN   CMS: 305963
// BSCS-9A SEECS

#include <iostream>
#include <sstream>
#include "TextCompressor.h"
#include "TextDecompressor.h"

using std::cout;
using std::string;
using std::cin;
using std::endl;
using std::ifstream;
using std::ofstream;


string compressedFileName(string inputName);
string decompressedFileName(string inputName);

int main()
{
    TextCompressor* tc = new TextCompressor();
    TextDecompressor* dc = new TextDecompressor();

    bool running = true;

    //running the program
    while (running)
    {
        string inputFileName;
        string outputFileName;
        int choice;//store users current choice

        //asking user what he wants to do
        system("CLS");
        cout << "---------------" << endl;
        cout << "TASK COMPRESSOR" << endl;
        cout << "---------------" << endl;

        cout << "Select Operation:" << endl;
        cout << "1. Compress a File." << endl;
        cout << "2. Decompress a File." << endl;
        cout << "3. End Program." << endl;
        cout << "Choice: ";

        cin >> choice;

        //doing what user wants
        switch (choice)
        {
        case 1:
            cout << "Enter Name of File to Compress: ";
            cin >> inputFileName;
            outputFileName = compressedFileName(inputFileName);
            if(outputFileName == "")
            {
                cout<<"Please select a .txt File"<<endl;
                cout << "Press Any Key to Continue" << endl;
                getchar();
                getchar();
                break;
            }
                cout << "Output file to be generated: "<< outputFileName <<endl;
            tc->compress(inputFileName.c_str(), outputFileName.c_str());
            cout << "Press Any Key to Continue" << endl;
            getchar();
            getchar();
            break;

        case 2:
            cout << "Enter Name of File to DeCompress: ";
            cin >> inputFileName;
            outputFileName = decompressedFileName(inputFileName);
            if(outputFileName == "")
            {
                cout<<"Please select a .tcs File"<<endl;
                cout << "Press Any Key to Continue" << endl;
                getchar();
                getchar();
                break;
            }
            cout << "Output file to be generated: "<< outputFileName<<endl;
            dc->decompress(inputFileName.c_str(), outputFileName.c_str());
            cout << "Press Any Key to Continue" << endl;
            getchar();
            getchar();
            break;

        case 3:
            running = false;
            break;

        case 4:
            cout << "Invalid Command. Try Again" << endl;
            break;
        }
    }
    cout << "Thank you for using TASK Compressor :)" << endl;
}

string compressedFileName(string inputName)
{

    std::stringstream ss(inputName);
    string filename;
    string extention;
    getline(ss,filename,'.');
    getline(ss,extention);
    if(extention != "txt")
        return "";
    return filename + ".tcs";
}

string decompressedFileName(string inputName)
{

    std::stringstream ss(inputName);
    string filename;
    string extention;
    getline(ss,filename,'.');
    getline(ss,extention);
    if(extention != "tcs")
        return "";
    return filename + "[Decompressed].txt";
}
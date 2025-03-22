#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "employee.cpp"

using namespace std;

void printBinaryFile(const char* filename) {
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cerr << "Error: Cannot open file " << filename << " for reading.\n";
        return;
    }

    cout << "\nBinary file content (" << filename << "):\n";
    cout << "--------------------------------\n";
    cout << "ID   Name       Hours\n";
    cout << "--------------------------------\n";

    Employee emp;
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        cout << emp.num << "   " << emp.name << "   " << emp.hours << "\n";
    }

    inFile.close();
}

void printReportFile(const char* filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error: Cannot open report file " << filename << " for reading.\n";
        return;
    }

    cout << "\nReport content (" << filename << "):\n";
    cout << "--------------------------------\n";

    string line;
    while (getline(inFile, line)) {
        cout << line << "\n";
    }

    inFile.close();
}

bool runProcess(const char* program, const vector<string>& args) {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Error: Failed to fork process.\n";
        return false;
    } 
    else if (pid == 0) { 
        vector<char*> c_args;
        c_args.push_back(const_cast<char*>(program));
        for (const auto& arg : args) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
        c_args.push_back(nullptr);

        execvp(program, c_args.data());
        cerr << "Error: Failed to execute " << program << "\n";
        exit(1);
    } 
    else { 
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

int main() {
    string binaryFile;
    int recordCount;

    cout << "Enter binary file name: ";
    cin >> binaryFile;

    cout << "Enter number of records: ";
    while (!(cin >> recordCount) || recordCount <= 0) {
        cout << "Invalid input. Enter a positive integer: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    if (!runProcess("./creator", {binaryFile, to_string(recordCount)})) {
        return 1;
    }

    printBinaryFile(binaryFile.c_str());

    string reportFile;
    double hourlyRate;

    cout << "\nEnter report file name: ";
    cin >> reportFile;

    cout << "Enter hourly rate: ";
    while (!(cin >> hourlyRate) || hourlyRate <= 0) {
        cout << "Invalid input. Enter a positive number: ";
        cin.clear();
        cin.ignore(10000, '\n');
    }

    if (!runProcess("./reporter", {binaryFile, reportFile, to_string(hourlyRate)})) {
        return 1;
    }

    printReportFile(reportFile.c_str());

    cout << "\nProgram finished successfully.\n";
    return 0;
}

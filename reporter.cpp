#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include "employee.cpp"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: Reporter <input_binary_file> <output_text_file> <hourly_rate>\n";
        return 1;
    }

    const char* inputFilename = argv[1];
    const char* outputFilename = argv[2];
    double hourlyRate = 0.0;

    try {
        hourlyRate = stod(argv[3]);
        if (hourlyRate <= 0) throw invalid_argument("Non-positive rate");
    } catch (...) {
        cerr << "Invalid hourly rate. Must be a positive number.\n";
        return 1;
    }

    ifstream inFile(inputFilename, ios::binary);
    if (!inFile) {
        cerr << "Error: Cannot open input file " << inputFilename << ".\n";
        return 1;
    }

    ofstream outFile(outputFilename);
    if (!outFile) {
        cerr << "Error: Cannot open output file " << outputFilename << " for writing.\n";
        return 1;
    }

    Employee emp;
    outFile << "Report for file: " << inputFilename << "\n";
    outFile << "--------------------------------------\n";
    outFile << "ID   Name       Hours   Salary\n";
    outFile << "--------------------------------------\n";

    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        double salary = emp.hours * hourlyRate;
        outFile << emp.num << "   " << emp.name << "   " << emp.hours << "   " << salary << "\n";
    }

    inFile.close();
    outFile.close();

    cout << "Report generated successfully: " << outputFilename << "\n";
    return 0;
}

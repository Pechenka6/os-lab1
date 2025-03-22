#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include "employee.cpp"

using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: Creator <output_binary_file> <num_records>\n";
        return 1;
    }

    const char* filename = argv[1];
    int numRecords = 0;

    try {
        numRecords = stoi(argv[2]);
        if (numRecords <= 0) throw invalid_argument("Non-positive records");
    } catch (...) {
        cerr << "Invalid number of records. Must be a positive integer.\n";
        return 1;
    }

    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Error: Cannot open file " << filename << " for writing.\n";
        return 1;
    }

    for (int i = 0; i < numRecords; i++) {
        Employee emp;
        cout << "Enter employee ID: ";
        cin >> emp.num;

        cout << "Enter employee name: ";
        cin >> emp.name;

        cout << "Enter hours worked: ";
        cin >> emp.hours;

        outFile.write(reinterpret_cast<char*>(&emp), sizeof(Employee));
    }

    outFile.close();
    cout << "Binary file created: " << filename << "\n";
    return 0;
}

#include <gtest/gtest.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream>
#include <vector>
#include <string>
#include <cstring>

using namespace std;

bool runProcess(const char* program, const vector<string>& args) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return false;
    } else if (pid == 0) {
        vector<char*> c_args;
        c_args.push_back(const_cast<char*>(program));
        for (const auto& arg : args) {
            c_args.push_back(const_cast<char*>(arg.c_str()));
        }
        c_args.push_back(nullptr);
        execvp(program, c_args.data());
        perror("Exec failed");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

TEST(ExternalCreatorAndReporterTest, FullWorkflowTest) {
    const string binFile = "test_employees.bin";
    const string reportFile = "test_report.txt";
    const int numRecords = 2;
    const double hourlyRate = 25.0;

    EXPECT_TRUE(runProcess("./creator", {binFile, to_string(numRecords)}));

    ifstream binCheck(binFile, ios::binary);
    ASSERT_TRUE(binCheck.is_open());
    binCheck.close();


    EXPECT_TRUE(runProcess("./reporter", {binFile, reportFile, to_string(hourlyRate)}));

    ifstream reportCheck(reportFile);
    ASSERT_TRUE(reportCheck.is_open());

    string line;
    bool hasContent = false;
    while (getline(reportCheck, line)) {
        if (!line.empty()) hasContent = true;
    }

    EXPECT_TRUE(hasContent);
    reportCheck.close();

    remove(binFile.c_str());
    remove(reportFile.c_str());
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
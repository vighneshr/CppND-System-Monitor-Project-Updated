#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() {
    /* process uptime */
    long int process_uptime = this->UpTime();
    string temp, line, utime, stime, cutime, cstime;
    std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid_) + LinuxParser::kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        int count = 0;
        while (count++ != 13) linestream >> temp;
        linestream >> utime >> stime >> cutime >> cstime;
    }
    long int total_time{stol(utime) + stol(stime) + stol(cutime) + stol(cstime)};
    
    return (total_time/process_uptime);
}

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
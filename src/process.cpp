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

// Done: Return this process's ID
int Process::Pid() { return pid_; }

// Done: Return this process's CPU utilization
float Process::CpuUtilization() {
    procJiffies = LinuxParser::ActiveJiffies(Pid());
    totalJiffies = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
    CPUUtil =
      (float)(procJiffies - prevProcJiffies) / (totalJiffies - prevTotalJiffies);
    prevTotalJiffies = totalJiffies;
    prevProcJiffies = procJiffies;
    return CPUUtil;
}

// Done: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// Done: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// Done: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// Done: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// Done: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return CPUUtil > a.CPUUtil;
}
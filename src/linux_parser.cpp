#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  float MemTotal, MemFree;
  string key, value, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "MemTotal:") {
          MemTotal = stof(value);
        }
        if (key == "MemFree:") {
          MemFree = stof(value);
        }
      }
    }
  }
  return (float)(MemTotal - MemFree)/MemTotal;
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() {
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return stol(uptime);
}

// Done: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Done: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
    string temp, line, utime, stime, cutime, cstime;
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        std:: cout << "pid:" << pid << " " << line.size() << std::endl;
        int count = 0;
        while (count++ != 12) linestream >> temp;
        linestream >> utime >> stime >> cutime >> cstime;
    }
    return (long)(stol(utime) + stol(stime) + stol(cutime) + stol(cstime));
}

// Done: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> val = CpuUtilization();
  return (stol(val[CPUStates::kUser_]) + stol(val[CPUStates::kNice_]) +
    stol(val[CPUStates::kSystem_]) + stol(val[CPUStates::kIRQ_]) +
    stol(val[CPUStates::kSoftIRQ_]) + stol(val[CPUStates::kSteal_]) +
    stol(val[CPUStates::kGuest_]) + stol(val[CPUStates::kGuestNice_]));
}

// Done: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> val = CpuUtilization();
  return stol(val[CPUStates::kIdle_]) + stol(val[CPUStates::kIOwait_]);
}

// Done: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> temp;
  string line, dummy;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)){
      std::istringstream linestream(line);
      while(linestream >> dummy){
        if (dummy == "cpu") {
          while(linestream >> dummy) temp.push_back(dummy);
          return temp;
        }
      }
    }
  }
  return temp;
}

// Done: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string msg, totalProc;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      if (line.find("processes") != std::string::npos) {
        std::istringstream linestream(line);
        linestream >> msg >> totalProc;
      }
    }
  }
  return stoi(totalProc);
}

// Done: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string msg, runningProc;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while(std::getline(filestream, line)) {
      if (line.find("procs_running") != std::string::npos) {
        std::istringstream linestream(line);
        linestream >> msg >> runningProc;
      }
    }
  }
  return stoi(runningProc);
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()){
    std::getline(filestream, line);
  }
  return line;
}

// Done: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string key, val, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if (line.find("VmSize:") != string::npos) {
        std::istringstream linestream(line);
        linestream >> key >> val;
        return val;
      }
    }
  }
  return val;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  string temp, uid, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      if (line.find("Uid") != std::string::npos) {
        std::istringstream linestream(line);
        linestream >> temp >> uid;
        return uid;
      }
    }
  }
  return uid;
}

// Done: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string uid = Uid(pid);
  string line, user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()){
    while(std::getline(filestream, line)) {
      if (line.find(uid) != std::string::npos) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> user;
      }
    }
  }
  return user;
}

// Done: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string uptimestr, line;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::stringstream linestream(line);
    int count = 1;
    while (linestream >> uptimestr && count < 22) count++;
  }
  long time(stol(uptimestr)/sysconf(_SC_CLK_TCK));
  return time;
}
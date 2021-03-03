#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>
#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) {}
  int Pid(); 
  std::string User(); 
  std::string Command(); 
  float CpuUtilization();
  std::string Ram();
  long int UpTime(); 
  bool operator<(Process const& a) const; 

  // necessary private members
 private:
 int pid_;
 long procJiffies{0};
 long totalJiffies{0};
 long prevProcJiffies{0};
 long prevTotalJiffies{0};
 float CPUUtil{0};

};

#endif
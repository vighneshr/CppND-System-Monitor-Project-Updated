#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long active = LinuxParser::ActiveJiffies();
    long idle = LinuxParser::IdleJiffies();
    long prevTotal = prevActive + prevIdle;
    long total = active + idle;
    long totalDelta = total - prevTotal;
    long idleDelta = idle - prevIdle;
    prevIdle = idle;
    prevActive = active;
    return (float)(totalDelta - idleDelta)/ totalDelta;
}
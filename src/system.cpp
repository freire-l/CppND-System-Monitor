#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "format.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
//using LinuxParser::CpuUtilization;
//to delete
using std::cout;

//github test

// Return the system's CPU
//First we call CpuUtilization from LinuxParses, which reads and parses the infro from /proc/stat and saves it into a vector
//We use that vector to feed the private members of cpu, that will be used to calculate the utilization
Processor& System::Cpu() { 

    cpu_.Utilization();
    return cpu_; 
}

// Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    processes_.clear();
    vector<int>  process_list = LinuxParser::Pids();
    
    for(auto i : process_list){
        Process new_proc(i);
        processes_.push_back(new_proc);
    }
    std::sort(processes_.begin(), processes_.end());
    return processes_; 
}

// Return the system's kernel identifier (string)
std::string System::Kernel() const{ return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() const { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
//long int System::UpTime() { Format::ElapsedTime(LinuxParser::UpTime()); } 
long int System::UpTime() const { return LinuxParser::UpTime(); }


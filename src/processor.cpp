#include "processor.h"
#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::vector;
using std::stof;
using LinuxParser::CPUStates;

// Return the aggregate CPU utilization

float Processor::Utilization() { 
    //SOURCE: = https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux 
 
    std::vector<string> cpu_info = LinuxParser::CpuUtilization();
    
    float Idle = stof(LinuxParser::str_check(cpu_info[CPUStates::kIdle_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kIOwait_]));

    float NonIdle = stof(LinuxParser::str_check(cpu_info[CPUStates::kUser_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kNice_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kSystem_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kIRQ_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kSoftIRQ_])) + stof(LinuxParser::str_check(cpu_info[CPUStates::kSteal_]));

    float Total = Idle + NonIdle;
    
    //COMMENTED BLOCK, I WAS TRYING TO GET A MORE PRECISE CPU UTILIZATION
    /*
    float Totald = Total - prev_Total_;
    float Idled = Idle - prev_Idle_;

    prev_Total_ = Total;
    prev_Idle_ = Idle;

    return (Totald - Idled)/Totald;            // --------------> Just by using this line instead on the one bellow I was getting this error
                                               // *** %n in writable segment detected *** Aborted (core dumped)   
                                               // Everything seems fine to me :( I have no clue why do I get it
                                               // If you have some spare time and want to fix this send me a PR 
                                               // 
    */
    return (Total - Idle)/Total; 
    
}

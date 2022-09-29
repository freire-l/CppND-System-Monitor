#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::str_check(string my_string){
  if(my_string!="")
    return my_string;
  else
    return "0";
  
}

// read data from the filesystem
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

// read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

//use std::filesystem
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
        int pid = stoi(str_check(filename));
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string key, value, unit;
  float mem_tot, mem_free;

  std::unordered_map <string, int> umap;

  //read the first 4 lines, and fill the unordered map with the labels and values
  int i = 3;
  std::ifstream filestream(kProcDirectory+kMeminfoFilename);
  if (filestream.is_open()) {
    while (i>=0) {
        std::getline(filestream, line);
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> key >> value >> unit;
        umap[key] =stoi(str_check(value));
        i--;
    }
  }
      mem_tot = float(umap.at("MemTotal"));
      mem_free = float(umap.at("MemFree"));

  return ((mem_tot-mem_free)/mem_tot);
}


// Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idle;
    return stol(str_check(uptime));
  }
  return 0;
}

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// Read and return the number of active jiffies for a PID

long LinuxParser::ActiveJiffies(int pid) { 

    string line;
    vector <string> parse;
    string value;

    std::ifstream stream(kProcDirectory +to_string(pid) + kStatFilename);

    if (stream.is_open()) {
      while(std::getline(stream, line)){
        std::istringstream linestream(line);
        //fill out the vector with values parsed from the line
        while (linestream >> value)
        {
          parse.push_back(value);
        }

         return stol(str_check(parse[13]))+stol(str_check(parse[14]))+stol(str_check(parse[15]))+stol(str_check(parse[16]));
      }
    }
  return 0; 
  
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
//Function that parses the first line of /proc/stat, and returns a vector with all its corresponding fields
vector<string> LinuxParser::CpuUtilization() {

  string name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
  }
  vector<string> cpu_info{name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
  return cpu_info; 

  }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    string line;
    string key, value;
    std::ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
      while(std::getline(stream, line)){
        std::istringstream linestream(line);
        linestream >> key >> value;
        if(key == "processes")
          return stoi(str_check(value));
      }
    }
  return 0; 
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string line;
    string key, value;
    std::ifstream stream(kProcDirectory + kStatFilename);

    if (stream.is_open()) {
      while(std::getline(stream, line)){
        std::istringstream linestream(line);
        linestream >> key >> value;
        if(key == "procs_running")
          return stoi(str_check(value));
      }
    } 
  return 0; 
}

// Read and return the command associated with a process

string LinuxParser::Command(int pid) { 
    string line;
    unsigned int char_limit = 40;
    string key, id1;
    std::ifstream stream(kProcDirectory +to_string(pid) + kCmdlineFilename);

    if (stream.is_open()) {
      std::getline(stream, line);
      if (line.size()>char_limit)
        return (line.substr(0,char_limit)+"...");
      else
        return line; //return the whole line
    }
  
  return string(); 
}

// Read and return the memory used by a process

string LinuxParser::Ram(int pid) { 

    string line;
    string key, id1;
    std::ifstream stream(kProcDirectory +to_string(pid) + kStatusFilename);

    if (stream.is_open()) {
      while(std::getline(stream, line)){
        std::istringstream linestream(line);
        //get tag and memory
        linestream >> key >> id1;
        //if(key == "VmSize:")
        //Using VmData instead of VmSize in order to display pysical RAM, as kindly suggested by a reviewer and can be seen here https://man7.org/linux/man-pages/man5/proc.5.html
        if(key == "VmData:")
          return id1;
      }
    }
  
  return string(); 
}

// Read and return the user ID associated with a process

string LinuxParser::Uid(int pid) { 
    
    string line;
    string key, id1;
    std::ifstream stream(kProcDirectory +to_string(pid) + kStatusFilename);

    if (stream.is_open()) {
      while(std::getline(stream, line)){
        std::istringstream linestream(line);
        //get tag and id
        linestream >> key >> id1;
        if(key == "Uid:")
          return id1;
      }
    }
    return string();
 }

// Read and return the user associated with a process

string LinuxParser::User(string uid) { 
  string line;
  string name, x, key;
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()){
    while (std::getline(filestream, line)) {
      //clean name
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> x >> key;
        if(key == uid){
          return name;
        }
    }
  }
  return string(); 

}

// Read and return the uptime of a process

long LinuxParser::UpTime(int pid) { 

     string line;
    vector <string> parse;
    string value;
    long up_time_sys;
    long time_pid_start;
    std::ifstream stream(kProcDirectory +to_string(pid) + kStatFilename);

    if (stream.is_open()) {
      while(std::getline(stream, line)){
        std::istringstream linestream(line);
        //parse all tokens
        while (linestream >> value)
        {
          parse.push_back(value);
        }
        //return only the uptime
        
         up_time_sys = LinuxParser::UpTime();
         time_pid_start  = (stol(str_check(parse[21]))/sysconf(_SC_CLK_TCK));
         return up_time_sys - time_pid_start;
        
      }
    }
  return 0; 
  
 
}

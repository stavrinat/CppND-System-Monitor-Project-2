#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
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

// TODO: Read and return the system memory utilization

float LinuxParser::MemoryUtilization() 
{ 
    string keyWord, MemTotal, MemFree,MemAvail, MemBuffers, unit,line;
    float totalUsedMemory = 0;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
  
    if (stream.is_open()) 
    {
       // total memory
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> keyWord >> MemTotal ;

    // free memory
    std::getline(stream, line);
    linestream.str(line);
    linestream >> keyWord >> MemFree;
      
        // available  memory
    std::getline(stream, line);
    linestream.str(line);
    linestream >> keyWord >> MemAvail;
      
        // memory buffers
    std::getline(stream, line);
    linestream.str(line);
    linestream >> keyWord >> MemBuffers ;
  }

  //https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
 //totalUsedMemory = stof(MemTotal) - stof(MemFree);
  totalUsedMemory = stof(MemTotal)-stof(MemFree);
  return totalUsedMemory/stof(MemTotal);

}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() 
{
  string uptime, idleprocess;
  string line;
  long upt;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> idleprocess;
  }
  
  upt = std::stol(uptime);
  return upt; 
}

// TODO: Read and return the number of jiffies for the system
//https://elinux.org/Kernel_Timer_Systems
//https://yaojingguo.github.io/2017/09/03/linux-kernel-time
long LinuxParser::Jiffies()
{
  string line;
  string keyWord,amount;
  long jif = 0;
  string mypath = kTimerList;
  std::ifstream filestream(mypath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> keyWord>> amount) {
        if (keyWord == "jiffies") {
          std::replace(amount.begin(), amount.end(), ':', ' ');
          jif = std::stol(amount);
          return jif;
        }
      }
    }
  }
  return jif;
} 

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//https://web.archive.org/web/20130302063336/http://www.lindevdoc.org/wiki//proc/pid/stat
//https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
long LinuxParser::ActiveJiffies(int pid) 
{
    string value,utime,stime,cutime,cstime;
  long activejiffies;
  string line;
   string mypath = kProcDirectory + std::to_string(pid) +kStatFilename;
  std::ifstream stream(mypath );
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
     for(int i=1; i<=13; i++)
      	linestream >> value;
     // not sure if that is the correct sum, maybe I should sum only utime and stime
      linestream>>utime>>stime>>cutime>>cstime;
      activejiffies = stol(utime)+stol(stime)+stol(cutime)+stol(cstime);
    }
  }
  
  return activejiffies; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
//http://www.linuxhowtos.org/System/procstat.htm
vector<string> LinuxParser::CpuUtilization()
{
   string keyWord, user,nice,system,idle,iowait,irq,softirq,steal;
  vector<string> cpuUtil;
  string line;
  string mypath = kProcDirectory + kStatFilename;
  std::ifstream stream(mypath);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> keyWord >> user>>nice>>system>>idle>>iowait>>irq>>softirq>>steal)
      {
        if(keyWord == "cpu")
        {
            cpuUtil = {user,nice,system,idle,iowait,irq,softirq,steal};
            break;
        }
      }
    }
  }
  return cpuUtil;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() 
{ 
  string keyWord, amount;
  int numOfProc = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> keyWord >> amount)
      {
        if(keyWord == "processes")
        {
            numOfProc = std::stoi(amount);
            return numOfProc;
        }
      }
    }
  }
  return numOfProc;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
  string keyWord, amount;
  int numOfProc = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> keyWord >> amount)
      {
        if(keyWord == "procs_running")
        {
            numOfProc = std::stoi(amount);
           return numOfProc;
        }
      }
    }
  }
  return numOfProc; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{ 
  
  string line;
  string mypath = kProcDirectory + std::to_string(pid) + kCmdlineFilename;
  std::ifstream stream(mypath);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
// mipos thelei prota na diairesoume tin mnimi me 1024 ? kai
// meta na tin kanoume ksana string?
//https://linux.die.net/man/5/proc
string LinuxParser::Ram(int pid) 
{ 
   string keyWord, amount;
  string vmsize;
  string line;
   string mypath = kProcDirectory + std::to_string(pid) +kStatusFilename;
  std::ifstream stream(mypath );
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> keyWord >> amount)
      {
        if(keyWord == "VmSize:")
        {
            vmsize = amount;
            return vmsize;
        }
      }
    }
  }
  return vmsize; 
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//https://linux.die.net/man/5/proc
string LinuxParser::Uid(int pid) 
{
 string keyWord, amount;
  string uid;
  string line;
   string mypath = kProcDirectory + std::to_string(pid) +kStatusFilename;
  std::ifstream stream(mypath );
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> keyWord >> amount)
      {
        if(keyWord == "Uid:")
        {
            uid = amount;
            return uid;
        }
      }
    }
  }
  return uid; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//https://www.cyberciti.biz/faq/understanding-etcpasswd-file-format/
string LinuxParser::User(int pid) 
{
   string username, password,userid;
  string user;
  string line;
   string mypath = kPasswordPath;
  std::ifstream stream(mypath );
  if (stream.is_open()) {
    string uid = LinuxParser::Uid(pid);
    while(std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      
      while(linestream >> username >> password >> userid)
      {
        if(uid == userid)
        {
            user = username;
            return user;
        }
      }
    }
  }
  return user; 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
//https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
long LinuxParser::UpTime(int pid) 
{
  string value;
  long uptime_pr;
  string line;
   string mypath = kProcDirectory + std::to_string(pid) +kStatFilename;
  std::ifstream stream(mypath );
  if (stream.is_open()) {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
     for(int i=1; i<=22; i++)
      	linestream >> value;
     
    }
  }
  
  uptime_pr = stol(value) / sysconf(_SC_CLK_TCK);
  return uptime_pr; 
}
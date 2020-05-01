#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include <linux_parser.h>
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() 
{
  vector<int> proc_ids = LinuxParser::Pids();
  processes_.clear();
  processes_.shrink_to_fit();
  for(int pid:proc_ids)
  {
    Process p (pid);
  	processes_ .push_back(p); 
  }
  return processes_; 
}

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() 
{ 
  kernel = LinuxParser::Kernel();
  return kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() 
{ 
  memUtil = LinuxParser::MemoryUtilization();
  return memUtil;
}

// TODO: Return the operating system name
std::string System::OperatingSystem() 
{ 
  OP = LinuxParser::OperatingSystem();
  return OP; 
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses()
{ 
  running_proc = LinuxParser::RunningProcesses();
  return running_proc;
}

// TODO: Return the total number of processes on the system
int System::TotalProcesses() 
{
  total_proc = LinuxParser::TotalProcesses();
  return total_proc;
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() 
{
  uptime = LinuxParser::UpTime();
  return uptime;
}
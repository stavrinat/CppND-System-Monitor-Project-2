#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// you forget to mention that we should also write a constructor :(
Process::Process(int pid)
{
  proc_pid = pid;
   proc_user =  LinuxParser::User(pid);
  proc_command = LinuxParser::Command(pid);
  proc_ram = LinuxParser::Ram(pid);
  proc_uptime = LinuxParser::UpTime(pid);
}


// TODO: Return this process's ID
int Process::Pid() 
{ 
  return proc_pid; 
}

// TODO: Return this process's CPU utilization
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
float Process::CpuUtilization() const
{ 
  float sys_uptime = LinuxParser::UpTime();
  
  float  proc_totalTime = LinuxParser::ActiveJiffies(proc_pid);
  float sec ;
  float cpuUtil;
  
  sec = sys_uptime - (proc_uptime/sysconf(_SC_CLK_TCK));
  cpuUtil = (proc_totalTime / sysconf(_SC_CLK_TCK)) / sec;
  return cpuUtil;

}

// TODO: Return the command that generated this process
string Process::Command() 
{ 
  return proc_command; 
}

// TODO: Return this process's memory utilization
string Process::Ram() 
{ 
  proc_ram = LinuxParser::Ram(proc_pid);
  return proc_ram; 
}

// TODO: Return the user (name) that generated this process
string Process::User() 
{ 
  return proc_user;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime()
{ 
  proc_uptime =LinuxParser::UpTime(proc_pid) ;
  return proc_uptime;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const 
{ 
  return CpuUtilization()< a.CpuUtilization();
}
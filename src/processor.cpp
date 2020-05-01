#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>
#include <iostream>


using std::string;
using std::vector;
using std::stof;


// TODO: Return the aggregate CPU utilization
//https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux/23376195#23376195
float Processor::Utilization() 
{ 
  vector<string> cpuUtil = LinuxParser::CpuUtilization();
  float user,nice,system,idle,iowait,irq,softirq,steal;
  float fidle, fprevIdle,fprevNonIdle,nonIdle,fprevTotal, total,totald,idled, CPU_Percentage;
  
  user = stof(cpuUtil[0]); 
  nice = stof(cpuUtil[1]); 
  system = stof(cpuUtil[2]); 
  idle = stof(cpuUtil[3]); 
  iowait = stof(cpuUtil[4]); 
  irq = stof(cpuUtil[5]); 
  softirq = stof(cpuUtil[6]); 
  steal = stof(cpuUtil[7]); 
  
  fprevIdle = prevIdle + prevIowait;
  fprevNonIdle = prevUser + prevNice + prevSystem + prevIrq + prevSoftirq + prevSteal;
  fprevTotal = fprevIdle + fprevNonIdle;

  fidle = idle + iowait;
  nonIdle = user + nice + system + irq + softirq + steal;
  total = fidle + nonIdle;
  
  totald = total - fprevTotal;
  idled = fidle - fprevIdle;
  CPU_Percentage = (totald - idled) / totald;
  
   prevUser = user;
   prevNice = nice;
   prevSystem = system;
   prevIdle = idle;
   prevIowait = iowait;
   prevIrq = irq;
   prevSoftirq = softirq;
   prevSteal = steal;
  
  return CPU_Percentage;
}
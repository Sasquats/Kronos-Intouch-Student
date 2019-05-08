//  Copyright 2017 Adrien Forkum
#include <boost/regex.hpp>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "ps7b.hpp"
//  Takes in two strings to parse out the time difference.
//  Returns the difference as a string.
std::string DeviceBU::time_d(std::string start_t, std::string end_t) {
std::string fStr, sStr, tStr;
std::stringstream ss;

int s_first = 0, s_second = 0, s_third = 0, e_first = 0, e_second = 0, e_third = 0;

for (int i = 0; i < static_cast<int>(start_t.size()); i++) {
 if (i > 10 && i < 13) {
  fStr.push_back(start_t[i]);
 } else if (i > 13 && i < 16) {
  sStr.push_back(start_t[i]);
 } else if (i > 16 && i < 19) {
  tStr.push_back(start_t[i]);
 }
}
s_first = atoi(fStr.c_str());
s_second = atoi(sStr.c_str());
s_third = atoi(tStr.c_str());
fStr.clear();
sStr.clear();
tStr.clear();

for (int i = 0; i < static_cast<int>(end_t.size()); i++) {
 if (i > 10 && i < 13) {
  fStr.push_back(end_t[i]);
 } else if (i > 13 && i < 16) {
  sStr.push_back(end_t[i]);
 } else if (i > 16 && i < 19) {
  tStr.push_back(end_t[i]);
 }
}
e_first = atoi(fStr.c_str());
e_second = atoi(sStr.c_str());
e_third = atoi(tStr.c_str());
fStr.clear();
sStr.clear();
tStr.clear();

s_first = e_first-s_first;
s_second = e_second-s_second;
s_third = e_third-s_third;
s_third += (s_first*360)+(s_second*60);
ss  << s_third*1000;
fStr = ss.str();
return fStr;
}

//  Takes in two strings one is the file to read from and the second is the file to write to.
//  Parses out the read file to create a report of the machine boot up and its services.
void file_rpt(std::string o_file, std::string w_file) {
int toggle = 0, line_num = 0, i_hold = 0;
std::string line, t_parse, t_hold, soft_s, soft_e, old_v, new_v;
std::ifstream file;
std::ofstream device;
device.open(w_file.c_str());
boost::smatch f_match, s_match, serv_match;
boost::cmatch t_match;
DeviceBU NewDev;

file.open(o_file.c_str());
if (file == NULL) {
 std::cout << "File failed to open!!" << std::endl;
}

boost::regex log("(.)+log.c.166(.)+");
boost::regex oej("(.)+.oejs.AbstractConnector.(.)+");
boost::regex time("([0-9]{4}-[0-9]{2}-[0-9]{2}.[0-9]{2}:[0-9]{2}:[0-9]{2})");
boost::regex softTime("[A-Z][a-z]{2}.(\\d){2}.(\\d){2}:(\\d){2}:(\\d){2}");
boost::regex s_start("Starting.Service(.)+");
boost::regex e_start("Service.started(.)+");
boost::regex s_time("\\d+.ms");
boost::regex service("(?<=\\.\\s)[^.]+\\s");
boost::regex softLoad("(.)+SOFTLOADSERVICE;Install.started(.)+");
boost::regex soft_version("(?<=base-)(.)+(?=\\.arm)");
boost::regex soft_new("(.)+Processing(.)+intouch-application-base(.)+");
boost::regex soft_old("(.)+removing.intouch-application-base(.)+");
boost::regex soft_end("(.)+ExitValue.from.install.command(.)+0");

// Loop through file for parsing.
while (std::getline(file, line)) {
 // Bootup start parse.
 if (boost::regex_match(line, f_match, log)) {
  if (toggle == 1) {
   NewDev.print(device);
   //NewDev.reset();
   toggle = 0;
  } else {
   if (boost::regex_search(line.c_str(),t_match, time)) {
    t_parse = t_match[0];
   }
   DeviceBU dev(line_num, o_file, t_parse);
   NewDev = dev;
   toggle = 1;
  } //  End of else.
 }//  End of outer if.

 // Service start parse.
 if(boost::regex_match(line, serv_match, s_start)) {
  t_hold = serv_match[0];
  boost::regex_search(t_hold, f_match, service);
  NewDev.push_service(line_num, f_match[0]);
 }

 //  Service end parse.
 if(boost::regex_match(line, serv_match, e_start)) {
  t_hold = serv_match[0];
  boost::regex_search(t_hold, serv_match, s_time);
  boost::regex_search(t_hold, f_match, service);
  t_hold = serv_match[0];
  i_hold = atoi(t_hold.c_str());
  NewDev.cmpl_service(line_num, f_match[0], i_hold);
 }
 //  Softload parse.
 if (boost::regex_match(line, serv_match, softLoad)) {
  if (boost::regex_search(line.c_str(), t_match, softTime)) {
   soft_s = t_match[0];
   NewDev.soft_start(line_num, soft_s);
  }
 }

 //  Softload old version.
 if (boost::regex_match(line, serv_match, soft_old)) {
  if(boost::regex_search(line.c_str(), t_match, soft_version)) {
   old_v = t_match[0];
  }
 }

 //  Softload new version.
 if (boost::regex_match(line, serv_match, soft_new)) {
  if(boost::regex_search(line.c_str(), t_match, soft_version)) {
   new_v = t_match[0];
   NewDev.soft_update(old_v, new_v);
  }
 }

 //  Softload end.
 if (boost::regex_match(line, serv_match, soft_end)) {
  if (boost::regex_search(line.c_str(), t_match, softTime)) {
   soft_e = t_match[0];
   NewDev.soft_end(line_num, soft_e);
  }
 }

 // Bootup end parse.
 if (boost::regex_match(line, s_match,oej)) {
  if (boost::regex_search(line.c_str(), t_match, time)) {
   t_hold = t_match[0];
   NewDev.boot_cmpl(line_num, t_hold);
  }//  End of inner if.
  toggle = 0;
  NewDev.print(device);
  //NewDev.reset();
 }//  End of outer if.
 line_num++;
}//  End of while.
file.close();
device.close();
}

void DeviceBU::print(std::ofstream &device){
device << "=== Device boot ===\n";
device << SLineNum << "(" << name << "): " << started << " Boot Start" << std::endl;
if (ELineNum == 0) {
 device << "**** Incomplete Boot ****\n\n";
} else {
 device << ELineNum << "(" << name << "): "
 << completed << " Boot Completed" << std::endl;
 device << "      " << time << "ms\n" <<std::endl;
}
device << "Services\n";
for (unsigned i = 0; i < services.size();i++) {
 if (services[i].isCmpl()) {
  device << "     " << services[i].getService() << std::endl; //  Print out service name
  device << "           Start: " << services[i].getSLine() << "(" << name << ")" << std::endl;  //  Start of service
  device << "           Completed: " << services[i].getELine() << "(" << name << ")" << std::endl;
  device << "           Elapsed Time: " << services[i].getTime() << " ms" << std::endl;
 }
}
device << "      ***Services not successfuly started: ";
for (unsigned i = 0; i < services.size();i++) {
 if (services[i].isCmpl() == 0) {
  device << services[i].getService() << " ";
 }
}
device << std::endl;
device << "===Softload===\n";
device << soft_begin << "(" << name << "): " << softSTime << " Softload Start\n";
device << "      Original version==>" << OVersion << std::endl;
device << "      New version==>" << NVersion << std::endl;
device << "      Elapsed time(sec)==>" << std::endl;
device << soft_stop << "(" << name << "): " << softETime << " Softload Completed\n";
}

void DeviceBU::cmpl_service(int LNum, std::string serv, int time){
for (unsigned i = 0;i < services.size(); i++) {
 if (services[i].getService() == serv) {
  services[i].setCmpl(LNum, time);
 }
}
}
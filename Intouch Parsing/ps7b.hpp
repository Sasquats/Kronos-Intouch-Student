//  Copyright 2017 Adrien Forkum
#ifndef PS7B_HPP
#define PS7B_HPP
#include <string>
#include <vector>
#include <iostream>

class Service{
public:
Service(int _SLineNum = 0, std::string  _service = "Sname") : service(_service), SLineNum(_SLineNum),
time(0), completed(0){}
std::string getService(){return service;}
int getSLine(){return SLineNum;}
int getELine(){return ELineNum;}
int getTime(){return time;}
void setCmpl(int LNum, int _time){ELineNum = LNum; time = _time; completed = 1;}
int isCmpl(){return completed;}

private:
std::string service;
int SLineNum, ELineNum, time, completed;
};

class DeviceBU{
public:
DeviceBU(int _BLineNum = 0, std::string _name = "Dname", std::string _started = "0"): name(_name),
started(_started), completed("00:00:00"), time("0"), softSTime("Jan 01 00:00:00"), softETime("Jan 01 00:00:00"),
SLineNum(_BLineNum), ELineNum(0), soft_begin(0), soft_stop(0){}
std::string time_d(std::string start_t, std::string end_t);
void boot_cmpl(int LNum, std::string cmpl){completed = cmpl; ELineNum = LNum;
time = time_d(started, completed);}
void push_service(int LNum, std::string serv){services.push_back(Service(LNum, serv));}
void cmpl_service(int LNum, std::string serv, int time);
void soft_start(int LNum, std::string stime){
soft_begin = LNum;
softSTime = stime;
std::cout << "Soft start" <<soft_begin << " " << softSTime << std::endl;
}
void soft_end(int LNum, std::string etime){soft_stop = LNum; softETime = etime;}
void soft_update(std::string old, std::string update){
OVersion = old;
NVersion = update;
std::cout << "Soft update" << OVersion << " " << NVersion << std::endl;
}
void reset(){services.clear(); completed = "0"; time = "0"; ELineNum = 0;}
void print(std::ofstream &device);

private:
std::string name, started, completed, time, softSTime, softETime, OVersion, NVersion;
std::vector<Service> services;
int SLineNum, ELineNum, soft_begin, soft_stop;
};

void file_rpt(std::string o_file, std::string w_file);

#endif
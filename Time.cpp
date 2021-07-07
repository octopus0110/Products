#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <thread>
using namespace std;

struct Time
{
 // 1マイクロ秒までの精度
 int hh, mm, ss, mls, mcs, nns;
 long long TIME;
 string time_s, scale;
 chrono::system_clock::time_point  t_start, t_elapse;

 Time(string s="seconds");

 string converttime2string();
 long long get_time();
 void print_time();
 long long start();
 long long elapse();
 string getDatetimeStr();
};

Time::Time(string s)
{
 scale = s;
 TIME = 0;
 time_s = "";
 hh = 0;
 mm = 0;
 ss = 0;
 mls = 0;
 mcs = 0;
 nns = 0;
 if (scale != "seconds" && scale != "milliseconds" && scale != "microseconds" && scale != "nanoseconds")
 {
  cerr << "scale is wrong" << endl;
  exit(-1);
 }
}

string Time::converttime2string()
{
 stringstream s;
 if (scale == "seconds")
 {
  ss = TIME % 60;
  mm = (TIME / 60) % 60;
  hh = TIME / 3600;
  s << setw(2) << setfill('0') << hh << ":";
  s << setw(2) << setfill('0') << mm << ":";
  s << setw(2) << setfill('0') << ss;
 }
 else if (scale == "milliseconds")
 {
  mls = TIME % 1000;
  ss = (TIME / 1000) %60;
  mm = (TIME / 60000) % 60;
  hh = TIME / 3600000;
  s << setw(2) << setfill('0') << hh << ":";
  s << setw(2) << setfill('0') << mm << ":";
  s << setw(2) << setfill('0') << ss << ":";
  s << setw(3) << setfill('0') << mls;
 }
 else if (scale == "microseconds")
 {
  mcs = TIME % 1000;
  mls = (TIME / 1000) % 1000;
  ss = (TIME / 1000000) % 60;
  mm = (TIME / 60000000) % 60;
  hh = TIME / 3600000000;
  s << setw(2) << setfill('0') << hh << ":";
  s << setw(2) << setfill('0') << mm << ":";
  s << setw(2) << setfill('0') << ss << ":";
  s << setw(3) << setfill('0') << mls << ":";
  s << setw(3) << setfill('0') << mcs;
 }
 else if (scale == "nanoseconds")
 {
  nns = TIME % 1000;
  mcs = (TIME / 1000) % 1000;
  mls = (TIME / 1000000) % 1000;
  ss = (TIME / 1000000000) % 60;
  mm = (TIME / 60000000000) % 60;
  hh = TIME / 3600000000000;
  s << setw(2) << setfill('0') << hh << ":";
  s << setw(2) << setfill('0') << mm << ":";
  s << setw(2) << setfill('0') << ss << ":";
  s << setw(3) << setfill('0') << mls << ":";
  s << setw(3) << setfill('0') << mcs << ":";
  s << setw(3) << setfill('0') << nns;
 }
 time_s = s.str();
 return time_s;
}

long long Time::get_time()
{
 if (scale == "seconds") TIME = chrono::duration_cast<chrono::seconds>(t_elapse-t_start).count();
 else if (scale == "milliseconds") TIME = chrono::duration_cast<chrono::milliseconds>(t_elapse-t_start).count();
 else if (scale == "microseconds") TIME = chrono::duration_cast<chrono::microseconds>(t_elapse-t_start).count();
 else if (scale == "nanoseconds") TIME = chrono::duration_cast<chrono::nanoseconds>(t_elapse-t_start).count();
 converttime2string();
 return TIME;
}

void Time::print_time()
{
 if (time_s != "") cout << time_s << endl;
 else cerr << "time_s is none" << endl;
}

long long Time::start()
{
 t_start = chrono::system_clock::now();
 t_elapse = t_start;
 get_time();
 return TIME;
}

long long Time::elapse()
{
 long long tmp;
 tmp = TIME;
 t_elapse = chrono::system_clock::now();
 get_time();
 if (tmp < TIME) return TIME;
 else return 0;
}

string Time::getDatetimeStr()
{
 time_t t = time(nullptr);
 const tm* localTime = localtime(&t);
 stringstream s;
 s << "20" << localTime->tm_year - 100;
 s << "/";
 // setw(),setfill()で0詰め
 s << setw(2) << setfill('0') << localTime->tm_mon + 1;
 s << "/";
 s << setw(2) << setfill('0') << localTime->tm_mday;
 s << "/";
 s << setw(2) << setfill('0') << localTime->tm_hour;
 s << ":";
 s << setw(2) << setfill('0') << localTime->tm_min;
 s << ":";
 s << setw(2) << setfill('0') << localTime->tm_sec;
 // stringにして値を返す
 return s.str();
}

void Sleep(const int& n)
{
 this_thread::sleep_for(std::chrono::milliseconds(n));
}

/*
int main()
{
 Time T("nanoseconds");
 T.start();
 T.print_time();
 while(T.ss < 66)
 {
  if (T.elapse() > 0)
  {
   //system("cls");
   cout << T.time << endl;
   T.print_time();
  }
 }

 return 0;
}
*/
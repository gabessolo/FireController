#ifndef __COMMON_H_
#define __COMMON_H_
/**/
#include <string>
#include <iomanip>
#include <cstdio>
#include <fstream>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <iostream>
#include <fstream>      
#include <iomanip>
#include <ctime>
#include <io.h>

using namespace std;

#define PORT_BUS      50
#define PORT_FIRE     75
#define PORT_FIRECTRL 60

#define MAX_FIRE      4
#define MAX_BUS       1
#define OTHER_FIRE    12
#define MAX_FIRECTRL  1
#define IP_BUS       "127.0.0.1"

#define REFRESH_TIMEOUT 10000 /* frequence d'émission des commandes */ 


#define TEMPO  500

enum typeMsg
{
	ALARM=-1,
	ABO,
	ACK,
	CMD,
	NOTIF_NEW_FIRE
};

enum typeClient
{
	FIRE=-1,
	FIRECTRL,
	JAVA,
	ARDUINO
};

enum typeColor
{
	RED=-1,
	GREEN,
	YELLOW,
};

struct _MSG
{
	byte  m_Timeout; /* seconds*/
	//typeCmd  m_Command;
	typeColor  m_Color;
	byte  m_Instance;
	char m_AdrIP[16];
	byte   m_Port;
	typeMsg  m_typeMsg;
	typeClient m_typeClient;
	byte m_Num;
	#ifdef WIN32
	SOCKET m_socketFire;
	SOCKET m_socketFireCtrl;
	SOCKET m_socketBus;
	#else
	int m_socketFire;
	int m_socketFireCtrl;
	int m_socketBus;
	#endif
	
};

static  const std::tm localtime_xp(std::time_t timer)
{
	std::tm bt{};
#if defined(__unix__)
	localtime_r(&timer, &bt);
#elif defined(_MSC_VER)
	localtime_s(&bt, &timer);
#else
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	bt = *std::localtime(&timer);
#endif
	return (const std::tm) bt;
}

static std::time_t time_now = std::time(nullptr);


static  string getCurrentDateTime(string s) {
	time_t now = time(0);
	struct tm  tstruct;
	char  buf[80];
	tstruct = localtime_xp(now);
	if (s == "now")
		strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
	else if (s == "date")
		strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);
	return string(buf);
};


#ifndef INFO
#define INFO  std::cout << std::put_time(&localtime_xp(time_now), "%y-%m-%d %OH:%OM:%OS") << " [INFO]  >> "
#endif

#ifndef ERROR
#define ERROR std::cout << std::put_time(&localtime_xp(time_now), "%y-%m-%d %OH:%OM:%OS") << " [ERROR] >> "
#else
#define _ERROR std::cout << std::put_time(&localtime_xp(time_now), "%y-%m-%d %OH:%OM:%OS") << " [ERROR] >> "

#endif

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

template <typename T>
static std::string NumberToString(T Number)
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

static void Logger(string logMsg) {

	string filePath = "log_" + getCurrentDateTime("date") + ".txt";
	string now = getCurrentDateTime("now");
	ofstream ofs(filePath.c_str(), std::ios_base::out | std::ios_base::app);
	ofs << now.c_str() << '\t' << logMsg.c_str() << '\n';
	ofs.close();
}

#endif /*  __COMMON_H_ */
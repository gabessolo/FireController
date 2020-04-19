#pragma once
#include <iostream>
using namespace std;
#include <vector>
#include <queue> 
#ifdef WIN32
#include <winsock.h>
#pragma comment(lib, "ws2_32.lib")
WORD wVersionRequested = MAKEWORD(2, 2);
WSADATA wsaData;
int err = WSAStartup(wVersionRequested, &wsaData);
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include <sstream>
#include <ctime>
#include <fstream>      // std::ofstream
#include "common.h"
#define _CRT_SECURE_NO_WARNINGS

extern std::time_t time_now;
extern /*inline*/ string getCurrentDateTime(string s);
extern /*inline*/ const std::tm localtime_xp(std::time_t timer);


namespace FireCtrlManagement
{

	struct _MSG
	{
		int  m_Timeout; // seconds
		//typeCmd  m_Command;
		typeColor  m_Color;
		int  m_Instance;
		char m_AdrIP[16];
		int   m_Port;
		typeMsg  m_typeMsg;
		typeClient m_typeClient;
#ifdef WIN32
		SOCKET m_socketFire;
		SOCKET m_socketFireCtrl;
		SOCKET m_socketBus;
#else
		int m_socketFire;
		int m_socketFireCtrl;
		int m_socketBus;
#endif
		int m_Num;
	};
	class CCOMCtrl
	{
	protected:
		int m_nbFire;
		int m_nbFireCtrl;
		int m_nbBus;
		bool m_bAck; // true si la demande d'abonnement est acquitté


		vector<struct _MSG> m_VecMsg;
		queue<struct _MSG>  m_QueueMsg;

		char* m_myName;
#ifdef WIN32
		SOCKET m_Socket;
		SOCKET m_SocketClient;
#else
		int m_Socket;
		int m_SocketClient;
#endif


#ifdef WIN32
		struct sockaddr_in m_destination;
		void SEND(SOCKET socket, struct _MSG& msg);
		void SEND2(SOCKET socket, struct _MSG& msg);
		virtual void receive_send(SOCKET socket, struct _MSG& msg) = 0;

#else
		struct sockaddr_in m_destination;
		void SEND(int socket, struct _MSG& msg);
		void SEND2(SOCKET socket, struct _MSG& msg);
		virtual void receive_send(int socket, struct MSG& msg) = 0;

#endif

		void close();
		void _connect(const char* ip, int port);
		void _connect();
		void initMsg(struct _MSG& msg);
		//
		virtual int init()
		{
			return 0;
		}

		//
		virtual void subscription()
		{
		
		}
		
		//
		virtual void Display(struct _MSG& msg)
		{

		}
		static DWORD WINAPI myThread(LPVOID arg)
		{
			CCOMCtrl *tr = (CCOMCtrl *)arg;
			tr->run();
			return 0;
		}

	public:

		void c_treatment()
		{
			DWORD dwThreadId = 0;
			HANDLE th = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myThread, (LPVOID)this, 0, &dwThreadId);
		}

		void  run();
		virtual const char* getName() = 0;

		CCOMCtrl()
		{
			m_nbFire = 0;
			m_nbFireCtrl = 0;
			m_nbBus = 0;
			m_bAck = false;
		}
	};

	class CFire : public CCOMCtrl
	{
	protected:
		static int m_Instance;
		int m_Num;
		void process();
#ifdef WIN32
		void  receive_send(SOCKET socket, struct _MSG& msg);
#else
		void  receive_send(int socket, struct _MSG& msg);
#endif
		int   init();
		void  subscription();
	public:
		//typeCmd m_Command;
		typeColor m_Color;
		const char* getName();
		CFire()
		{
			m_nbFireCtrl = -1;
			m_nbBus = -1;
			m_Instance++;
			m_Num = m_Instance;
			if (m_Instance== 1)
				m_Color= GREEN;
			if (m_Instance == 2)
				m_Color = GREEN;
			if (m_Instance == 3)
				m_Color =  YELLOW;
			if (m_Instance == 4)
				m_Color = YELLOW;
			//m_Command = NDEF;
		}
		virtual ~CFire() { ; }

	};


	class CBus : public CCOMCtrl
	{
	protected:
	
	virtual void Display(struct _MSG& msg);

	int init();
	#ifdef WIN32
		void  receive_send(SOCKET socket, struct _MSG& msg);
		SOCKET m_Socket2;
	#else
		void  receive_send(int socket, struct _MSG& msg);
		int m_Socket2;
	#endif
		void process();
		void subscription();

	public:
		const char* getName();
		CBus()
		{
			m_nbFire = 0;
			m_nbFireCtrl = 0;
			m_nbBus = 1;

		}
		void _connect2(const char* ip, int port);
		virtual ~CBus() { ; }


	};


	class CFireControl : public CCOMCtrl
	{
	protected:
		static int m_Instance;
		bool m_bFull;
		int m_Num;
		int init();
#ifdef WIN32
		void  receive_send(SOCKET socket, struct _MSG& msg);
#else
		void receive_send(int socket, struct _MSG& msg);
#endif
		void  subscription();
		void process();


	public:
		const char* getName();
		CFireControl()
		{
			m_nbFireCtrl = -1;
			m_nbBus = -1;
			m_bFull = m_bAck = false;
			m_Instance++;
			m_Num = m_Instance;
		}
		virtual ~CFireControl();
		bool isFull() { return m_nbFire < MAX_FIRE; }
	};


	//thread qui va seconder le FireController
	class CommandHandler:public CCOMCtrl
	{
	protected:
		struct _MSG m_Msg;
		vector<struct _MSG>& m_VecMsg;
		void process();

		static DWORD WINAPI myThread(LPVOID arg)
		{
			CommandHandler *tr = (CommandHandler *)arg;
			tr->process();
			return 0;
		};

	public:

		void c_treatment()
		{
			DWORD dwThreadId = 0;
			HANDLE th = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)myThread, (LPVOID)this, 0, &dwThreadId);
		}

		CommandHandler(vector<struct _MSG>& vecMsg) : m_VecMsg(vecMsg)
		{
		}
		
		void receive_send(SOCKET socket, struct _MSG& msg)
		{
		}
		void _connect(const char* ip, int port);
		void _connect2(const char* ip, int port);

		#ifdef WIN32
		SOCKET m_Socket;
		SOCKET m_Socket2;
		struct sockaddr_in m_destination;
		void SEND(SOCKET socket, struct _MSG& msg);
		#else
		int m_Socket;
		int m_Socket2;
		struct sockaddr_in m_destination;
		void SEND(int socket, struct _MSG& msg);
		#endif

		virtual ~CommandHandler()
		{

		}

		const char* getName() { return "CmdHandler "; }
	};
}


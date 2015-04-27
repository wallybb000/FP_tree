#ifndef SOCKETH
#define SOCKETH

#pragma comment(lib, "Ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
using namespace std;

//TODO:將socketClass 分成兩種  使server可同時連接多個client

class SocketClass
{
	enum Type { Server, Client };
public:

	Type SocketType;


	SOCKET sListen; //listening for an incoming connection
	SOCKET sConnect[10]; //operating if a connection was found

	SOCKADDR_IN addr;
	SOCKADDR_IN clinetAddr[10];

	int clinetCount;

	SocketClass(int type);
	
	void Listen();
	
	void Accept();
	void Send(char * message, int len, int clinetId);


	void Connect();
	void receive(char ** message, int &len);

};
#endif
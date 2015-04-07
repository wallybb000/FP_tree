
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

	SocketClass(int type) :clinetCount(0)
	{
		int r;
		WSAData wsaData;
		WORD DLLVSERION;
		DLLVSERION = MAKEWORD(2, 1);//Winsocket-DLL 版本
		//用 WSAStartup 開始 Winsocket-DLL
		r = WSAStartup(DLLVSERION, &wsaData);



		if (type == 0)
		{
			SocketType = Server;
		}
		else if (type == 1)
		{
			SocketType = Client;
		}


		addr.sin_addr.s_addr = inet_addr("127.0.0.1");
		addr.sin_family = AF_INET;
		addr.sin_port = htons(1234);

		for(int i= 0;i<10 ;i++)
		sConnect[i] = socket(AF_INET, SOCK_STREAM, NULL);
		sListen = socket(AF_INET, SOCK_STREAM, NULL);


	}
	void Listen()
	{
		sListen = socket(AF_INET, SOCK_STREAM, NULL);
		bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
		listen(sListen, SOMAXCONN);
	}
	void Accept(char * message )
	{
		if (SocketType != Server)return;

		while (true)
		{
			cout << "waiting..." << endl;

		int addrlen = sizeof(addr);
		if (sConnect[clinetCount] = accept(sListen, (SOCKADDR*)&clinetAddr[clinetCount], &addrlen))
		{
			cout << "a connection was found" << endl;
			printf("server: got connection from %s\n", inet_ntoa(clinetAddr[clinetCount].sin_addr));
			printf("clinet NO is %d\n", clinetCount);

			//傳送訊息給 client 端
			send(sConnect[clinetCount], message, (int)strlen(message), 0);
			

			clinetCount++;
		}
		}
	}
	void Connect(char * message )
	{

		if (SocketType != Client)return;

		connect(sConnect[0], (SOCKADDR*)&addr, sizeof(addr));
		int messagelen = sizeof(message);
		//接收 server 端的訊息
		ZeroMemory(message, 10000000);
		recv(sConnect[0], message, 10000000, 0);
		cout << "Maessage get" << endl;


	}
};
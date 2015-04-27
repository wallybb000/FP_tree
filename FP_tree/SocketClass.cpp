#include "SocketClass.h"

SocketClass::SocketClass(int type) :clinetCount(0)
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

	for (int i = 0; i < 10; i++)
		sConnect[i] = socket(AF_INET, SOCK_STREAM, NULL);
	sListen = socket(AF_INET, SOCK_STREAM, NULL);


}
void SocketClass::Listen()
{
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
}
void SocketClass::Accept()
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

			clinetCount++;


			
		}

		if (clinetCount == 3) break;


	}
}

void SocketClass::Send(char * message, int len, int clinetId )
{
	//傳送訊息給 client 端
	
	cout<<"1:" <<send(sConnect[clinetId], (char*)&len, sizeof(len), 0);
	cout<<"2:" <<send(sConnect[clinetId], message, len, 0);
}

void SocketClass::Connect()
{

	if (SocketType != Client)return;
	connect(sConnect[0], (SOCKADDR*)&addr, sizeof(addr));
	cout << "client connecting" << endl;


}
void SocketClass::receive(char ** message ,int &len)
{

	//接收 server 端的訊息
	
	recv(sConnect[0], (char*)&len,sizeof(len) , 0);
	(*message) = new char[len]; 
	//ZeroMemory(message, len);
	recv(sConnect[0], *message, len, 0);
	cout << "Maessage get" << endl;


}
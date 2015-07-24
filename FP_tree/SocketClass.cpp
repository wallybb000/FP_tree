#include "SocketClass.h"

SocketClass::SocketClass() 
{
	int r;
	WSAData wsaData;
	WORD DLLVSERION;
	DLLVSERION = MAKEWORD(2, 1);//Winsocket-DLL 版本
	//用 WSAStartup 開始 Winsocket-DLL
	r = WSAStartup(DLLVSERION, &wsaData);




	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);


	sConnect = socket(AF_INET, SOCK_STREAM, NULL);
	sListen = socket(AF_INET, SOCK_STREAM, NULL);


}
void SocketClass::setType(int type)
{
	if (type == 0)
		SocketType = Server;
	else if (type == 1)
		SocketType = Client;
}
void SocketClass::Listen()
{
	sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
}
SOCKET& SocketClass::Accept()
{
	if (SocketType == Server){
	SOCKET *clientSocket = new SOCKET();
	std::cout << "waiting..." << endl;

	SOCKADDR_IN clientAddr;


	clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(1235);

	int addrlen = sizeof(clientAddr);
	if ((*clientSocket)=accept(sListen, (SOCKADDR*)&clientAddr, &addrlen))
	{
		std::cout << "a connection was found" << endl;
		printf("server: got connection from %s\n", inet_ntoa(clientAddr.sin_addr));
	}

	return (*clientSocket);
}
}

void SocketClass::Connect()
{

	if (SocketType != Client)return;
	connect(sConnect, (SOCKADDR*)&addr, sizeof(addr));
	std::cout << "client connecting" << endl;


}
void SocketClass::Send(SOCKET &clientSocket,char * message, int len )
{
	//傳送訊息給 client 端
	
	std::cout<<"1:" <<send(clientSocket, (char*)&len, sizeof(len), 0);
	std::cout<<"2:" <<send(clientSocket, message, len, 0);
}


void SocketClass::receive(SOCKET &clientSocket,char ** message ,int &len)
{

	//接收 server 端的訊息
	
	recv(clientSocket, (char*)&len, sizeof(len), MSG_WAITALL);
	(*message) = new char[len]; 
	//&ZeroMemory(message, len);
	recv(clientSocket, *message, len, MSG_WAITALL);
	std::cout << "Maessage get" << endl;


}
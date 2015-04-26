#include "SocketClass.h"

SocketClass::SocketClass(int type) :clinetCount(0)
{
	int r;
	WSAData wsaData;
	WORD DLLVSERION;
	DLLVSERION = MAKEWORD(2, 1);//Winsocket-DLL ����
	//�� WSAStartup �}�l Winsocket-DLL
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
void SocketClass::Accept(char * message)
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

			//�ǰe�T���� client ��
			send(sConnect[clinetCount], message, (int)strlen(message), 0);


			clinetCount++;
		}
	}
}
void SocketClass::Connect(char * message)
{

	if (SocketType != Client)return;
	
	connect(sConnect[0], (SOCKADDR*)&addr, sizeof(addr));
	int messagelen = sizeof(message);
	//���� server �ݪ��T��
	ZeroMemory(message, 10000000);
	recv(sConnect[0], message, 10000000, 0);
	cout << "Maessage get" << endl;


}
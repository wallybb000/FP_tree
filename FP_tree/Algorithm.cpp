#include "Algorithm.h"
int Algorithm::typeId;
FP_Tree Algorithm::mFP_Tree;
mutex Algorithm::mFPT_Mutex;
ofstream Algorithm::out;
DB_Scanner Algorithm::dataBase;
SocketClass Algorithm::mSocketClass;

int Algorithm::minSup;
int Algorithm::AmountOfClient;
string Algorithm::fileName;

///////////////////////
int Algorithm::computingId;


void Algorithm::askUser()
	{
		cout << "Select  server(1)  or  client(2) : " << endl;
		cin >> typeId;
		if (typeId == 1)
		{
			cout << "Input the Database filename and Minimum Supple:" << endl;
			cin >> fileName;
			cin >> minSup;
			cout << "How many Clinets?" << endl;
			cin >> AmountOfClient;
		}
		else
		{
			cout << "computing id:" << endl;
			cin >> computingId;
		}
	}
void Algorithm::doEach()
	{
		if (typeId == 1)
			server();
		else
			client();
	}

void Algorithm::server()
	{
		dataBase.setFileName(fileName);
		dataBase.setMinSup(minSup);
		dataBase.firstCheck();
		dataBase.openBinaryDatabase();

		mFP_Tree.setMinSup(minSup);

		mSocketClass.setType(0);
		mSocketClass.Listen();

		int initValue = 1;
		list<int> inputItemList;
		while (dataBase.readLine(inputItemList))
			mFP_Tree.insertNode(inputItemList, initValue);

		mFP_Tree.Loop();

		
				
		for (;;){
			// 等待直到客戶端連接進來
			std::thread mThread(cast, mSocketClass.Accept());
			mThread.join();
		}
		
	}

void Algorithm::cast(SOCKET &clientSocket)
	{


		stringstream FPTreeSerial;
		mFPT_Mutex.lock();
		mFP_Tree.Serialization(FPTreeSerial);
		mFPT_Mutex.unlock();

		mSocketClass.Send(clientSocket, const_cast<char*>(FPTreeSerial.str().c_str()), FPTreeSerial.str().size());

		//得長度
		char *resultTemp;
		int sizeOfResultTemp;
		mSocketClass.receive(clientSocket, &resultTemp, sizeOfResultTemp);

		//
		static int fileNo = 0;
		ofstream result(to_string(fileNo) + ".txt");
		result.write(resultTemp, sizeOfResultTemp);
		result.close();
		
	}
	
void Algorithm::client()
	{
		mSocketClass.setType(1);
		mSocketClass.Connect();
		
		cout << "read" << endl;
		char * temp;
		int sizeOfTemp;
		mSocketClass.receive(mSocketClass.sConnect, &temp, sizeOfTemp);

		stringstream FPTreeSerial(string(temp,sizeOfTemp));
		mFP_Tree.insertNodeFromSerial(FPTreeSerial);
		mFP_Tree.Loop();
		cout << "write" << endl;
		//todo : 完成回傳結果
		ifstream tempe(string("temp.txt"));
		stringstream Serial;
		Serial << tempe.rdbuf();
		int sizeOfTempe = Serial.str().size();
		tempe.close();
		mSocketClass.Send(mSocketClass.sConnect, (char*)Serial.str().c_str(), Serial.str().size());

	}



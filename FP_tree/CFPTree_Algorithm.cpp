#include "CFPTree_Algorithm.h"
int CFPTree_Algorithm::typeId;
FP_Tree CFPTree_Algorithm::mFP_Tree;
mutex CFPTree_Algorithm::mFPT_Mutex;
ofstream CFPTree_Algorithm::out;
DB_Scanner CFPTree_Algorithm::dataBase;
SocketClass CFPTree_Algorithm::mSocketClass;

int CFPTree_Algorithm::minSup;
int CFPTree_Algorithm::AmountOfClient;
string CFPTree_Algorithm::fileName;

///////////////////////
int CFPTree_Algorithm::computingId;


void CFPTree_Algorithm::askUser()
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
void CFPTree_Algorithm::doEach()
	{
		if (typeId == 1)
			server();
		else
			client();
	}

void CFPTree_Algorithm::server()
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

		//mFP_Tree.Loop();
		
		vector<FP_Tree * > allCFPTree;
		ofstream ff("temp.txt");
		mFP_Tree.DivideByLoopOnce(allCFPTree ,&ff);
		
		for (int i = 0; i < allCFPTree.size(); i++)
			allCFPTree[0]->insertFromTree(*allCFPTree[i]);

		allCFPTree[0]->Loop();


		//allCFPTree[0]->insertFromTree(mFP_Tree);
		
				
		for (;;){
			// 等待直到客戶端連接進來
			std::thread mThread(cast, mSocketClass.Accept());
			mThread.join();
		}
		
	}

void CFPTree_Algorithm::cast(SOCKET &clientSocket)
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
		ofstream result(to_string(fileNo++) + ".txt");
		result.write(resultTemp, sizeOfResultTemp);
		result.close();
		
	}
	
void CFPTree_Algorithm::client()
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



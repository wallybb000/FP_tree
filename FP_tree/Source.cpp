#include "Node.h"
#include "Header.h"
#include "FP_tree.h"
#include "DB_Scanner.h"
#include "Algorithm.h"
#include <ctime>
clock_t start, stop;

void main()
{
	Algorithm::askUser();
	Algorithm::doEach();


};

/*
//	 ofstream out;
//void main()
//{
//	cout << "Select  server(1)  or  client(2) : " << endl;
//	int select;
//	cin >> select;


//	FP_Tree mFP_Tree;
//	if (select == 1){

//		SocketClass mSocketClass(0);
//		double count;
//		char file[1000];

//		cout << "Input the Database filename:" << endl;
//		cin >> file;
//		cout << "Input Minimum Supple" << endl;
//		cin >> count;
//		
//		/*********************************************************/
//		DB_Scanner db(file, 0);
//		cout << "Database first loading..." << endl;
//		db.firstCheck();
//		db.minSup = count;

//		out.open("out.txt", ios::trunc);
//		mFP_Tree.setMinSup(count);
//		
//		cout << mFP_Tree.getMinSup() << endl;

//		/*********************************************************/
//		cout << "Data second loading..." << endl;
//		int initValue = 1;

//		db.openBinaryDatabase();
//		list<int> inputItemList;
//		while (db.readLine(inputItemList))
//			mFP_Tree.insertNode(inputItemList, initValue);



//		/*********************************************************/
//		cout << "Starting looping" << endl;
//		start = clock();
//		{
//			mFP_Tree.Loop();
//		}
//		stop = clock();

//		out << endl << endl <<"Running Time :"<< (double)(stop - start) / CLOCKS_PER_SEC;
//		out.close();

//		
//		/*********************************************************/

//		cout << "Starting communication..." << endl;

//		stringstream FPTreeSerial;
//	
//		mFP_Tree.Serialization(FPTreeSerial);
//		mSocketClass.Listen();
//		mSocketClass.Accept();
//		for(int i =0 ;i<3;i++)
//		mSocketClass.Send(const_cast<char*>(FPTreeSerial.str().c_str()), FPTreeSerial.str().size(), i);
//	}
//	
//	else if(select ==2)
//	{
//		int computingId;
//		cout << "computing id:" << endl;
//		cin >> computingId;
//		SocketClass mSocketClass(1);
//		string fileName("out");
//		fileName += to_string(computingId) + "txt";
//		out.open(fileName, ios::trunc);

//		
//		/*********************************************************/
//		cout << "Starting communication..." << endl;
//		mSocketClass.Listen();
//		mSocketClass.Connect();
//		int serialLen;
//		char *Buffer;
//		mSocketClass.receive(&Buffer,serialLen);

//		/*********************************************************/
//		stringstream FPTreeSerial(string(Buffer,serialLen));


//		cout << "Building FP_Tree..." << endl;
//		mFP_Tree.insertNodeFromSerial(FPTreeSerial);

//		/*********************************************************/
//		cout << "Starting FP_Tree looping..." << endl;
//		start = clock();
//		{
//			mFP_Tree.Loop();
//		}
//		stop = clock();
//		
//		out << endl << endl <<"Running Time :"<< (double)(stop - start) / CLOCKS_PER_SEC;
//		out.close();

//	}



//	system("pause");

//}


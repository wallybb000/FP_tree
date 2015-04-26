#include "Node.h"
#include "SocketClass.h"
#include "Header.h"
#include "FP_tree.h"
#include "DB_Scanner.h"
#include <ctime>

clock_t start, stop;

 ofstream out;
void main()
{
	cout << "Select  server(1)  or  client(2) : " << endl;
	int select;
	cin >> select;


	FP_Tree mFP_Tree;
	if (select == 1){

		SocketClass mSocketClass(0);
		char file[1000];
		cout << "Input the Database filename:" << endl;
		cin >> file;
		double count;
		cout << "Input Minimum Supple" << endl;
		cin >> count;

		cout << "Database first loading..." << endl;
		DB_Scanner db(file, 0);
		db.firstCheck();
		db.minSup = db.Total_Transaction*count+0.5;

		out.open("out.txt", ios::trunc);
		mFP_Tree.setMinSup(db.Total_Transaction*count+0.5);
		
		cout << mFP_Tree.getMinSup() << endl;

		cout << "Data second loading..." << endl;
		int initValue = 1;

		db.openBinaryDatabase();
		list<int> inputItemList;
		while (db.readLine(inputItemList))
			mFP_Tree.insertNode(inputItemList, initValue);



		cout << "Starting looping" << endl;
		start = clock();
		{
			mFP_Tree.Loop();
		}
		stop = clock();

		out << endl << endl <<"Running Time :"<< (double)(stop - start) / CLOCKS_PER_SEC;
		out.close();

		

		cout << "Starting communication..." << endl;
		string FPTreeSerial;

		FPTreeSerial = to_string(mFP_Tree.getMinSup()) + "\n";
		FPTreeSerial += mFP_Tree.Serialization();
		
		mSocketClass.Listen();
		mSocketClass.Accept(const_cast<char*>(FPTreeSerial.c_str()));

	}
	
	else if(select ==2)
	{

		SocketClass mSocketClass(1);
		out.open("out2.txt", ios::trunc);
		char * Buffer = new char[10000000];

		
		cout << "Starting communication..." << endl;
		mSocketClass.Listen();
		mSocketClass.Connect(Buffer);


		cout << "Building FP_Tree..." << endl;
		mFP_Tree.insertNodeFromSerial(string(Buffer));

		cout << "Starting FP_Tree looping..." << endl;
		start = clock();
		{
			mFP_Tree.Loop();
		}
		stop = clock();
		
		out << endl << endl <<"Running Time :"<< (double)(stop - start) / CLOCKS_PER_SEC;
		out.close();

	}

}


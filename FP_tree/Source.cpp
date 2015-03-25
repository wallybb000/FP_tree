#include "Node.h"

#include <ctime>

clock_t start, stop;

 ofstream out;
void main()

{
	char file[1000];
	cin >> file;
	int count;
	cin >> count;

	out.open("out.txt", ios::trunc);
	DB_Scanner db(file,0);
	db.firstCheck();
	db.minSup = db.Total_Transaction*count;
	FP_Tree mFP_Tree;
	mFP_Tree.setMinSup(db.Total_Transaction*count);

	int initValue=1;
	list<int> inputItemList;
	while (db.readLine(inputItemList))
	{
		mFP_Tree.insertNode(inputItemList , initValue);
	}

	string temp;
	temp = mFP_Tree.Serialization();
	cout << temp;

	system("PAUSE");
	start = clock();

	mFP_Tree.Loop();
		

	

	stop = clock();

//	cout << ++in[610];
	out <<endl<<endl<< (double)(stop - start) /CLOCKS_PER_SEC;
	out.close();
	
	



}


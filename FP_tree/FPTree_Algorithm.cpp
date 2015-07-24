#include "FPTree_Algorithm.h"
void FPTree_Algorithm::askUser()
{
	cout << "Select  server(1)  or  client(2) : " << endl;
	cin >> typeId;
	if (typeId == 1)
	{
		cout << "Input the Database filename and Minimum Supple:" << endl;
		cin >> fileName;
		cin >> minSup;
		mDB.setFileName(fileName);
		mDB.setMinSup(minSup);

	}
	else
	{
		cout << "computing id:" << endl;
		cin >> computingId;
	}
}

void FPTree_Algorithm::doEach()
{
	if (typeId == 1)
		server();
	else
		client();
}
void FPTree_Algorithm::server(){

	mOfstream.open("out.txt", ios::trunc);
	mFP_Tree.setMinSup(this->minSup);
	mFP_Tree.setOstream(&mOfstream);

	/*********************************************************/
	cout << "Database first loading..." << endl;
	mDB.firstCheck();
	/*********************************************************/
	cout << "Data second loading..." << endl;
	int initValue = 1;

	mDB.openBinaryDatabase();
	list<int> inputItemList;
	while (mDB.readLine(inputItemList))
		mFP_Tree.insertNode(inputItemList, initValue);



	/*********************************************************/
	cout << "Starting looping" << endl;
	clock_t start, stop;
	{
		start = clock();
		mFP_Tree.Loop();
		stop = clock();
	}

	mOfstream << endl << endl
		<< "Running Time :" << (double)(stop - start) / CLOCKS_PER_SEC;
	mOfstream.close();


	/*********************************************************/
	cout << "Starting communication..." << endl;

	stringstream FPTreeSerial;
	mFP_Tree.Serialization(FPTreeSerial);

	mSocketClass.setType(0);
	mSocketClass.Listen();

		mSocketClass.Send(mSocketClass.Accept(), const_cast<char*>(FPTreeSerial.str().c_str()), FPTreeSerial.str().size());
}

void FPTree_Algorithm::client(){

	cout << "computing id:" << endl;
	cin >> computingId;

	string fileName("out");
	fileName += to_string(computingId) + "txt";
	mOfstream.open(fileName, ios::trunc);
	mFP_Tree.setOstream(&mOfstream);

	/*********************************************************/
	cout << "Starting communication..." << endl;
	int serialLen;
	char *Buffer;
	mSocketClass.setType(1);
	mSocketClass.Listen();
	mSocketClass.Connect();
	mSocketClass.receive(mSocketClass.sConnect, &Buffer, serialLen);

	/*********************************************************/
	stringstream FPTreeSerial(string(Buffer, serialLen));


	cout << "Building FP_Tree..." << endl;
	mFP_Tree.insertNodeFromSerial(FPTreeSerial);

	/*********************************************************/
	cout << "Starting FP_Tree looping..." << endl;
	clock_t start, stop;
	{
		start = clock();
		mFP_Tree.Loop();
		stop = clock();
	}

	mOfstream << endl << endl
		<< "Running Time :" << (double)(stop - start) / CLOCKS_PER_SEC;
	mOfstream.close();


}
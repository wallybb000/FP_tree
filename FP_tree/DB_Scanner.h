
#ifndef DBSCANNER
#define DBSCANNER

#include"Header.h"

using namespace std;
class DB_Scanner
{
public:
	ifstream inFile;
	ifstream inFileBinary;
	int Total_Transaction;
	string fileName;
	double minSup;
	int limit;

	int allItemTable[15000];

	DB_Scanner(char * inFileName, double minsup);

	void firstCheck();
	bool openBinaryDatabase();
	bool readLine(list<int> &inArr);
	void closeBinaryDatabase();

};
#endif

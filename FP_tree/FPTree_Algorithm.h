#ifndef FPTREE_ALGORITHM
#define FPTREE_ALGORITHM
#include "Header.h"
#include "FP_tree.h" 
#include "DB_Scanner.h"
#include "SocketClass.h"
using namespace std;
class FPTree_Algorithm
{
private:

	int typeId;
	int minSup;
	int computingId;
	string fileName;
	

	ofstream mOfstream;
	FP_Tree mFP_Tree;
	DB_Scanner mDB;
	SocketClass mSocketClass;
	////////////////////////////
public:
	void askUser();
	void doEach();

	void server();
	void client();
	
};





#endif
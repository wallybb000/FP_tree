
#ifndef CFALGORITHM
#define CFALGORITHM
#include"Header.h"
#include"FP_tree.h"
#include "SocketClass.h"
#include "DB_Scanner.h"
#include <thread>
#include <mutex>
//using namespace boost::asio;


// for CFPTree Algorithm
class CFPTree_Algorithm{
public:
	static int typeId;
	static FP_Tree mFP_Tree;
	static mutex mFPT_Mutex;
	static ofstream out;
	static DB_Scanner dataBase;
	static SocketClass mSocketClass;

	static int minSup;
	static int AmountOfClient;
	static string fileName;

	//////////////////////////////
	static int computingId;

	static void askUser();
	static void doEach();
	
	static void server();
	static void cast(SOCKET &clientSocket);
	static void client();
	



};
#endif
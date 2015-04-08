#include <vector>
#include <map>
#include <list>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <queue>
using namespace std;


	extern ofstream out;
class Node
{
public:

	typedef std::pair<int, Node *  > IDandNode;
	typedef std::map<int, Node *> IDtoNode;
	int nodeId;//該node的唯一編號
	int itemId;//該node中的Item編號(非唯一)
	int value;
	IDtoNode childByItem;
	int childAmount;
	int high;
	Node* parent;
	Node* Next;
public:
	Node(int NId, int ItemID, Node * Parent) 
		:nodeId(NId), itemId(ItemID), parent(Parent), childAmount(0), value(-1)
	{};

	void insertChild(Node * input)
	{
		childByItem.insert(IDandNode(childAmount, input));
		childAmount++;
	};


	~Node()
	{
	}

};



class FP_Tree{


	typedef map < int, list<Node *> > ItemidToList;

	struct ValueAndList
	{
		ValueAndList() :value(0){};
		ValueAndList(int iValue, list<Node*> iList) :mList(iList), value(iValue){};
		int value;
		list < Node * > mList;

	};
	typedef map< int, ValueAndList> VaildItem;

	class StringAndValue
	{
	public:

		StringAndValue() :str(), value(0){};
		StringAndValue(string mStr, int iValue) :str(mStr), value(iValue){};
		string str;
		int value;
	};
	class bucket
	{
	public:
		const int initValue = -1;
		vector<int> arr;
		list <int> hasBeInsert;

		bucket(int Amount) :arr(Amount, initValue)
		{
		}

		void insert(int index, int value)
		{
			arr[index] = value;
			hasBeInsert.push_back(index);
		}
		void earse(int index)
		{
			arr[index] = initValue;
		}
		void reset()
		{
			for (list<int>::iterator it = hasBeInsert.begin(); it != hasBeInsert.end(); ++it)
			{
				arr[*it] = initValue;
			}
			hasBeInsert.clear();
		}
	};

private:
	int MinSup;

	auto_ptr<Node>  NodePool;
	int NodeAmount;
	map<int, int> NodeValueList;


	VaildItem EachItemList;
	const int MaxNodes = 1000;
	const int MaxItems = 500;
public:
	bool isStraight;
	string TreeName;

	FP_Tree() :NodePool(new Node(-1, -1, NULL)), isStraight(true)
	{
		NodeAmount = 0;

	};
	FP_Tree(int Minsup, string  &name)
		:NodePool(new Node(-1, -1, NULL)), NodeAmount(0), MinSup(Minsup), TreeName(name), isStraight(true)
	{
	};

	void setMinSup(int i){ MinSup = i; }
	int getMinSup(){ return MinSup; }
	void insertNode(list<int> &inputList, int &value);
	void Loop();
	string Serialization();
	void insertNodeFromSerial(string & SerialString);

};
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

	DB_Scanner(char * inFileName, double minsup)
		: Total_Transaction(0), limit(0), minSup(minsup),fileName(inFileName)
	{
		for (int i = 0; i < 15000; i++)
		{

			allItemTable[i] = 0;
		}
	};

	void firstCheck()
	{
		inFile.open(fileName, ios::in);

		string newFileName(fileName);
		newFileName = "S_" + newFileName;
		ofstream out(newFileName.c_str(), ios::binary);
		
		if (!inFile.is_open()|| !out.is_open()) return;

		int temp=0;
		char inputC;
		int buf_index = 0;
		while (!inFile.eof())
		{

			int buf[1000];
			

			inputC = inFile.get();
			if (inputC  >47)
			{
				temp *= 10;
				temp += inputC - 48;
			}
			else if (inputC == ',')
			{
				++allItemTable[temp];
				buf[++buf_index] = temp;
				temp = 0;
			}
			else if (inputC == '\n')
			{
				//存入buf
				++allItemTable[temp];
				buf[++buf_index] = temp;
				temp = 0;
				//將buf存入二進制
				
				buf[0] = buf_index ;
				out.write((char *)&buf, sizeof(int)*(buf_index+1));
				++Total_Transaction;
				buf_index = 0;
			}
		}

		inFile.close();

	}

	bool openBinaryDatabase()
	{
		string newFileName(fileName);
		newFileName = "S_" + newFileName;
		inFileBinary.open(newFileName.c_str(), ios::binary);
		if (inFileBinary.is_open() && !inFileBinary.eof())return true;
		else
			return false;
	}

	bool readLine(list<int> &inArr)
	{
		if (!inFileBinary.is_open()||inFileBinary.eof())return false;
		
		inArr.clear();
		int count;
		inFileBinary.read((char *)& count, sizeof(count));

		for (int i = 0; i < count; i++)
		{

			int buf;
			inFileBinary.read((char *)& buf, sizeof(buf));

			inArr.push_back(buf);
		}

		return true;
		
	}
	void closeBinaryDatabase()
	{
		inFileBinary.close();
	}
};
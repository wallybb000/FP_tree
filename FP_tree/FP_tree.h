#ifndef FPTREEH
#define FPTREEH

#include "Header.h"
#include "Node.h"

#define AMOUNT_OF_CHILDRENTABLE 200000

struct ValueAndPtrList
{
	ValueAndPtrList() :value(0){};
	ValueAndPtrList(int iValue, list<Node*> iList) :mList(iList), value(iValue){};
	int value;
	list < Node * > mList;
};


struct ValueAndIntList
{
	ValueAndIntList() :value(0){};
	ValueAndIntList(int iValue ,list<int> iList) :mList(iList), value(iValue){};
	int value;
	list <int> mList;
};

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

class FP_Tree{


	typedef map< int, ValueAndPtrList> VaildItem;

private:
	int MinSup;
	int NodeAmount;
	int ItemCount;//For CFPTree
	auto_ptr<Node>  NodePool;
	ostream * pOstream;
	


	VaildItem ItemTable;
	const int MaxNodes = 1000;
	const int MaxItems = 500;

public:
	string TreeName;
	FP_Tree() :NodePool(new Node(-1, -1, NULL)), pOstream(NULL)
	{
		NodeAmount = 0;
	};
	FP_Tree(int Minsup, string  &name,ostream* ipOstream)
		:NodePool(new Node(-1, -1, NULL)), NodeAmount(0), MinSup(Minsup), TreeName(name), pOstream(ipOstream)
	{
	};
	~FP_Tree(){
		
		for(auto & item :ItemTable)
			for (auto& ptr : item.second.mList)
			{
				Node* temp = ptr;
				delete temp;

			}

	}

	void setOstream(ostream * ipOstream){ this->pOstream = ipOstream; }
	void setMinSup(int i){ MinSup = i; }
	int getMinSup(){ return MinSup; }
	int getItemCount(){ return ItemCount; }
	

	void insertNode(list<int> &inputList, int value);
	int upScanFromItem(ValueAndPtrList &item,vector<ValueAndIntList> & buildingListArr);
	void Loop();
	

	// for Network communication
	void Serialization(stringstream & SerialString);
	void insertNodeFromSerial(stringstream & SerialString);



	//for CFPTree
	void insertFromTree(FP_Tree & inFPTree);
	void DivideByLoopOnce(vector<FP_Tree*> & subFPTree_Arr,ofstream * outStream);
	void mergeLoop(Node & A,Node & B);

};






#endif

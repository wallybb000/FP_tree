#ifndef FPTREEH
#define FPTREEH

#include "Header.h"
#include "Node.h"

#define AMOUNT_OF_CHILDRENTABLE 200000

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

#endif

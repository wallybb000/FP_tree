#ifndef NODEH
#define NODEH
#include "Header.h"

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

	IDtoNode & dchildByItem;
	IDtoNode childByItem;
	int childAmount;
	int high;
	Node* parent;
	Node* Next;
public:
	Node(int NId, int ItemID, Node * Parent)
		:nodeId(NId), itemId(ItemID), parent(Parent), childAmount(0), value(-1), dchildByItem(childByItem){
	};

	~Node(){
	}

	void insertChild(Node * input){
		childByItem.insert(IDandNode(childAmount, input));
		childAmount++;
	};



};
#endif

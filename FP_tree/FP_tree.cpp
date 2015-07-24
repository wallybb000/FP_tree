#include "Header.h"
#include "Node.h"
#include "FP_tree.h"
bool compare(FP_Tree * i, FP_Tree *j) { return ( i->getItemCount() > j->getItemCount() ); }

void FP_Tree::insertNode(list<int> &inputList, int value)
{
	Node * Temp = NodePool.get();
	list<int>::iterator it_List = inputList.begin();
	for (; it_List != inputList.end(); it_List++)
	{
		//a: 檢查Temp當前點是否已有同一點

		int itemId = *it_List;
		Node::IDtoNode::iterator it_Next = Temp->childByItem.find(itemId);

		if (it_Next == Temp->childByItem.end())
		{
			//b: 加入input 並設定該點數量,

			Node * newNode = new Node(NodeAmount, itemId, NULL);
			Temp->childByItem.insert(Node::IDandNode(itemId, newNode));
			newNode->value = value;
			NodeAmount++;

			//判斷是否為直樹
			//if (Temp->childByItem.size() > 1)isStraight = false;


			//將該點加入eachItemList
			VaildItem::iterator it_ItemList = ItemTable.find(itemId);
			if (it_ItemList == ItemTable.end())
			{
				ValueAndPtrList newValueAndPtrList;
				newValueAndPtrList.mList.push_back(newNode);
				newValueAndPtrList.value = value;
				ItemTable.insert(pair<int, ValueAndPtrList  >(itemId, newValueAndPtrList));

			}
			else
			{
				ItemTable[itemId].mList.push_back(newNode);
				ItemTable[itemId].value += value;
			}



			Node* father = Temp;
			Temp = newNode;//c: 將Temp移至該點
			newNode->parent = father;
		}
		else
		{

			//b:將Temp 移至該點 
			VaildItem::iterator it_ItemList = ItemTable.find(itemId);
			if (it_ItemList == ItemTable.end())
			{
				cout << ":fjeofijeifjeoif";
			}
			it_ItemList->second.value += value;
			Temp = it_Next->second;
			Temp->value += value;
		}
	}
};

int FP_Tree::upScanFromItem(ValueAndPtrList &item, vector<ValueAndIntList> & buildingListArr)
{
	int index_ListArr = 0;

	//計算個別Item在該樹中所有Node的value總和
	bucket sumOfItemValue(100000);
	int itemValueSum = 0;


	//從當前迴圈中item的mList中 挑出所屬的每一Node 一一處理(爬升)
	list<Node*>::iterator it_List = item.mList.begin();
	for (; it_List != item.mList.end(); it_List++)
	{

		Node* bottonNode = *it_List;//爬升前最底下的Node 
		Node* curNode = bottonNode;
		buildingListArr[index_ListArr].value = bottonNode->value;
		itemValueSum += bottonNode->value;

		//開始爬升
		while (1)
		{
			curNode = curNode->parent;
			if (curNode == NodePool.get())break;

			//將值加於新的VaildItem
			if (sumOfItemValue.arr[curNode->itemId] != -1)
			{
				sumOfItemValue.arr[curNode->itemId] += bottonNode->value;
			}
			else
			{
				sumOfItemValue.insert(curNode->itemId, bottonNode->value);
			}

			//生成建樹串列群

			buildingListArr[index_ListArr].mList.push_front(curNode->itemId);

		}
		//計算該次爬升的長度 並找出屢次的最大值
		++index_ListArr;

	}
	if (itemValueSum < MinSup)return -1;
	//移除掉建樹串列群中的不合格item
	for (int index = 0; index < index_ListArr; index++)
	{
		list<int> ::iterator it_List = buildingListArr[index].mList.begin();
		for (; it_List != buildingListArr[index].mList.end();)
			//若該item之value總和未達MinSup 刪之
			if (sumOfItemValue.arr[*it_List] < MinSup)
				it_List = buildingListArr[index].mList.erase(it_List);
			else
				it_List++;
	}

	return itemValueSum;
};
void FP_Tree::Loop()
{

	//從可用的Item表中 一一處理
	VaildItem::reverse_iterator it_VaildItem = ItemTable.rbegin();
	for (; it_VaildItem != ItemTable.rend(); it_VaildItem++)
	{
		vector<ValueAndIntList> buildingListArr(it_VaildItem->second.mList.size());

		int itemValueSum = upScanFromItem(it_VaildItem->second, buildingListArr);

		if (itemValueSum == -1)continue;

		//將當前的搜尋的資訊輸出

		string str1 = TreeName;
		str1 += "-";
		str1 += to_string(it_VaildItem->first);
		*pOstream << str1 << ":" << to_string(itemValueSum) << endl;


		//建立新的子樹
		FP_Tree mNewFP_Tree(MinSup, str1,this->pOstream);
		for (int index = 0; index < buildingListArr.size(); index++)
			mNewFP_Tree.insertNode(buildingListArr[index].mList, buildingListArr[index].value);

		mNewFP_Tree.Loop();

	}





}

void FP_Tree::DivideByLoopOnce(vector<FP_Tree*> & subFPTree_Arr,ofstream * outStream)
{
	//從可用的Item表中 一一處理
	VaildItem::iterator it_VaildItem = ItemTable.begin();
	for (; it_VaildItem != ItemTable.end(); it_VaildItem++)
	{
		vector<ValueAndIntList> buildingListArr(it_VaildItem->second.mList.size());

		int itemValueSum = upScanFromItem(it_VaildItem->second, buildingListArr);

		if (itemValueSum == -1)continue;
		
		//將當前的搜尋的資訊輸出
		string str1 = TreeName;
		str1 += "-";
		str1 += to_string(it_VaildItem->first);
		if (outStream != NULL)
			*outStream << str1 << ":" << to_string(itemValueSum) << endl;
		str1 = TreeName;//移除子樹標頭名

		for (auto & buildingList : buildingListArr)
		{
			if (buildingList.value > 1)
				cout << "ddd";

			if (buildingList.mList.size() > 0)
				buildingList.mList.push_back(it_VaildItem->first);
			else
				itemValueSum -= buildingList.value;


			cout << "";
		}

		
		
		//建立新的子樹
		FP_Tree* mNewFP_Tree = new FP_Tree(MinSup, str1,this->pOstream);
		for (int index = 0; index < buildingListArr.size(); index++)
		{
			if (buildingListArr[index].mList.size()>0)
			mNewFP_Tree->insertNode(buildingListArr[index].mList, buildingListArr[index].value);
		}
		//填入該itemsum 
		mNewFP_Tree->ItemCount = itemValueSum;
		if (mNewFP_Tree->getItemCount() >=MinSup )
		subFPTree_Arr.push_back(mNewFP_Tree);
	}
	
	sort(subFPTree_Arr.begin(), subFPTree_Arr.end(), compare);

}

void FP_Tree::Serialization(stringstream& SerialString)
{

	SerialString.write((char*)&this->MinSup, sizeof(this->MinSup));
	SerialString.write((char*)&NodeAmount, sizeof(NodeAmount));

	queue<Node *> NodeQueue;
	NodeQueue.push(NodePool.get());

	while (1)
	{
		if (NodeQueue.empty())break;

		Node * curNode = NodeQueue.front();
		NodeQueue.pop();

		int nodeId = -1;
		if (curNode->parent != NULL)
		{
			nodeId = curNode->nodeId;
			SerialString.write((char*)&nodeId, sizeof(nodeId));
			SerialString.write((char*)&curNode->parent->nodeId, sizeof(curNode->parent->nodeId));
			SerialString.write((char*)&curNode->itemId, sizeof(curNode->itemId));
			SerialString.write((char*)&curNode->value, sizeof(curNode->value));

		}
		Node::IDtoNode::iterator it_Child = curNode->childByItem.begin();

		for (; it_Child != curNode->childByItem.end(); it_Child++)
			NodeQueue.push(it_Child->second);

	}

}

void FP_Tree::insertNodeFromSerial(stringstream & SerialString)
{

	int nodeAmount;
	int minSup;

	SerialString.read((char*)&minSup, sizeof(minSup));
	SerialString.read((char*)&nodeAmount, sizeof(int));
	this->setMinSup(minSup);
	//this->NodeAmount = nodeAmount;
	Node **nodePtrArr = new Node*[nodeAmount]{NULL};

	for (int NodeIndex = 0; NodeIndex < nodeAmount; NodeIndex++)
	{
		int nodeID, parentID, value, itemID;
		Node * parentPointer;
		SerialString.read((char*)&nodeID, sizeof(nodeID));
		SerialString.read((char*)&parentID, sizeof(parentID));
		SerialString.read((char*)&itemID, sizeof(itemID));
		SerialString.read((char*)&value, sizeof(value));
		if (parentID == -1)
			parentPointer = NodePool.get();
		else
			parentPointer = nodePtrArr[parentID];

		nodePtrArr[nodeID] = new Node(nodeID, itemID, parentPointer);
		nodePtrArr[nodeID]->value = value;

		parentPointer->insertChild(nodePtrArr[nodeID]);
		this->NodeAmount++;

		VaildItem::iterator it_ItemList = ItemTable.find(itemID);
		if (it_ItemList == ItemTable.end())
		{
			ValueAndPtrList newValueAndPtrList;
			newValueAndPtrList.mList.push_back(nodePtrArr[nodeID]);
			newValueAndPtrList.value = nodePtrArr[nodeID]->value;
			ItemTable.insert(pair<int, ValueAndPtrList  >(itemID, newValueAndPtrList));

		}
		else
		{
			ItemTable[itemID].mList.push_back(nodePtrArr[nodeID]);
			ItemTable[itemID].value += nodePtrArr[nodeID]->value;
		}



	}

	delete[] nodePtrArr;

}


void FP_Tree::insertFromTree(FP_Tree & inFPTree){

	mergeLoop(*this->NodePool.get(), *inFPTree.NodePool.get());
}


void FP_Tree::mergeLoop(Node & A,Node & B){
	
	struct id_And_isABFound
	{
		int id;
		bool A;
		bool B;
		
	};

	vector <id_And_isABFound> AB_ChildTable;
	AB_ChildTable.reserve(A.childByItem.size() + B.childByItem.size());
	for (auto & A_Child : A.childByItem)
	{
		id_And_isABFound temp={0,true,false};
		temp.id = A_Child.second->itemId;
		AB_ChildTable.push_back(temp);
	}
	for (auto & B_Child : B.childByItem)
	{
		bool isFind = false;
		for (auto & in : AB_ChildTable)
			if (B_Child.second->itemId == in.id)
			{
				isFind = true;
				in.B = true;
				break;
			}
		if (isFind == false)
		{
		id_And_isABFound temp={0,false,true};
			temp.id = B_Child.second->itemId;
			AB_ChildTable.push_back(temp);
		}
	}
	///////////////////////////////////////////////////////////////////////////
	for (auto &itemIdAndAppear : AB_ChildTable)
	{
		if (itemIdAndAppear.A  && !itemIdAndAppear.B )
			continue;
		else if (!itemIdAndAppear.A && itemIdAndAppear.B)
		{
			Node * B_ChildNode = B.childByItem.find(itemIdAndAppear.id)->second;
			Node * A_newNode = new Node(NodeAmount++, itemIdAndAppear.id, &A);
			
			A_newNode->value =B_ChildNode->value;
			
			//將newNode插入到樹中
			A.insertChild(A_newNode);
			this->ItemTable[A_newNode->itemId].mList.push_back(A_newNode);
			this->ItemTable[A_newNode->itemId].value += A_newNode->value;

			mergeLoop(*A_newNode, *B_ChildNode);
		}
		
		else if ((itemIdAndAppear.A) && (itemIdAndAppear.B))
		{
			A.value = max(A.value, B.value);
			Node * A_ChildNode = A.childByItem.find(itemIdAndAppear.id)->second;
			Node * B_ChildNode = B.childByItem.find(itemIdAndAppear.id)->second;
			
			mergeLoop(*A_ChildNode, *B_ChildNode);

		}

		
	}



}


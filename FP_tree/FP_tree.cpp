#include "Header.h"
#include "Node.h"
#include "FP_tree.h"


void FP_Tree::insertNode(list<int> &inputList, int &value)
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
			if (Temp->childByItem.size() > 1)isStraight = false;


			//將該點加入eachItemList
			VaildItem::iterator it_ItemList = EachItemList.find(itemId);
			if (it_ItemList == EachItemList.end())
			{
				ValueAndList newValueAndList;
				newValueAndList.mList.push_back(newNode);
				newValueAndList.value = value;
				EachItemList.insert(pair<int, ValueAndList  >(itemId, newValueAndList));

			}
			else
			{
				EachItemList[itemId].mList.push_back(newNode);
				EachItemList[itemId].value += value;
			}



			Node* father = Temp;
			Temp = newNode;//c: 將Temp移至該點
			newNode->parent = father;
		}
		else
		{

			//b:將Temp 移至該點 
			VaildItem::iterator it_ItemList = EachItemList.find(itemId);
			if (it_ItemList == EachItemList.end())
			{
				cout << ":fjeofijeifjeoif";
			}
			it_ItemList->second.value+=value;
			Temp = it_Next->second;
			Temp->value += value;
		}
	}
};

void FP_Tree::upScanFromItem(ValueAndList & vaildItem)
{
};
void FP_Tree::Loop()
{

	//從可用的Item表中 一一處理
	VaildItem::reverse_iterator it_VaildItem = EachItemList.rbegin();
	for (; it_VaildItem != EachItemList.rend(); it_VaildItem++)
	{
		//建立子樹的串列與該串列的value
		vector<list<int>> subTree_ListArr(it_VaildItem->second.mList.size());
		vector<int> subTree_Listvalue(it_VaildItem->second.mList.size());
		
		//subTree_ListArr的index
		int index_ListArr = 0;

		//計算個別Item在該樹中所有Node的value總和
		bucket sumOfItemValue(100000);
		int sumBotton = 0;


		//從當前迴圈中item的mList中 挑出所屬的每一Node 一一處理(爬升)
		list<Node*>::iterator it_List = it_VaildItem->second.mList.begin();
		for (; it_List != it_VaildItem->second.mList.end(); it_List++)
		{
			
			Node* bottonNode = *it_List;//爬升前最底下的Node 
			Node* curNode = bottonNode;
			subTree_Listvalue[index_ListArr] = bottonNode->value;
			sumBotton += bottonNode->value;
			
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

				subTree_ListArr[index_ListArr].push_front(curNode->itemId);

			}
			//計算該次爬升的長度 並找出屢次的最大值
			++index_ListArr;

		}

		if (sumBotton < MinSup)continue;
		//移除掉建樹串列群中的不合格item
		for (int index = 0; index < index_ListArr; index++)
		{
			list<int> ::iterator it_List = subTree_ListArr[index].begin();
			for (; it_List != subTree_ListArr[index].end();)
				//若該item之value總和未達MinSup 刪之
				if (sumOfItemValue.arr[*it_List] < MinSup)
					it_List = subTree_ListArr[index].erase(it_List);
				else
					it_List++;
		}

		//將當前的搜尋的資訊輸出
		string str1 = TreeName;
		static ofstream out("temp.txt");
		str1 += " - ";
		str1 += to_string(it_VaildItem->first);
		out << str1 << "  value :" << to_string(sumBotton) << endl;

		//建立新的子樹
		FP_Tree mNewFP_Tree(MinSup, str1);
		for (int index = 0; index < index_ListArr; index++)
		{
			mNewFP_Tree.insertNode(subTree_ListArr[index], subTree_Listvalue[index]);
		}

		

		//若該不為直樹 則繼續loop
		//if (!mNewFP_Tree.isStraight)
		{
			mNewFP_Tree.Loop();
		}
		/*
		else
		{
			list<string> stringList;
			list<Node *> resultList;
			Node * ptr = mNewFP_Tree.NodePool.get();
			for (;;){
				if (ptr->childByItem.size() == 1)
				{

					ptr = ptr->childByItem.begin()->second;
					resultList.push_front(ptr);
					stringList.push_front(to_string(ptr->itemId));
				}
				else if (ptr->childByItem.size() == 0)
				{
					break;
				}
				else
				{
					cout << "error";
				}
			}
			string str2 = str1;
			list<Node *>::reverse_iterator it_ResultList1 = resultList.rbegin();
			for (; it_ResultList1 != resultList.rend(); it_ResultList1++)
			{
				str1 += " - ";
				str1 += to_string((*it_ResultList1)->itemId);
				out << str1 << "  value :" << to_string(sumBotton) << endl;
			}




		}
		*/
		
	}





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
		this->setMinSup( minSup);
		//this->NodeAmount = nodeAmount;
		Node **nodePtrArr = new Node*[nodeAmount]{NULL};
		
		for (int NodeIndex = 0; NodeIndex < nodeAmount; NodeIndex++)
		{
			int nodeID, parentID, value, itemID;
			Node * parentPointer;
			SerialString.read((char*)&nodeID   , sizeof(nodeID));
			SerialString.read((char*)&parentID , sizeof(parentID));
			SerialString.read((char*)&itemID   , sizeof(itemID));
			SerialString.read((char*)&value, sizeof(value));
			if (parentID == -1)
				parentPointer = NodePool.get();
			else
				parentPointer = nodePtrArr[parentID];

			nodePtrArr[nodeID] = new Node(nodeID, itemID, parentPointer);
			nodePtrArr[nodeID]->value = value;

			parentPointer->insertChild(nodePtrArr[nodeID]);
			this->NodeAmount++;
			
			VaildItem::iterator it_ItemList = EachItemList.find(itemID);
			if (it_ItemList == EachItemList.end())
			{
				ValueAndList newValueAndList;
				newValueAndList.mList.push_back(nodePtrArr[nodeID]);
				newValueAndList.value = nodePtrArr[nodeID]->value;
				EachItemList.insert(pair<int, ValueAndList  >(itemID, newValueAndList));

			}
			else
			{
				EachItemList[itemID].mList.push_back(nodePtrArr[nodeID]);
				EachItemList[itemID].value += nodePtrArr[nodeID]->value;
			}



		}
		
		delete[] nodePtrArr;

	}


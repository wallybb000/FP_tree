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
		//a: �ˬdTemp��e�I�O�_�w���P�@�I

		int itemId = *it_List;
		Node::IDtoNode::iterator it_Next = Temp->childByItem.find(itemId);

		if (it_Next == Temp->childByItem.end())
		{
			//b: �[�Jinput �ó]�w���I�ƶq,

			Node * newNode = new Node(NodeAmount, itemId, NULL);
			Temp->childByItem.insert(Node::IDandNode(itemId, newNode));
			newNode->value = value;
			NodeAmount++;

			//�P�_�O�_������
			//if (Temp->childByItem.size() > 1)isStraight = false;


			//�N���I�[�JeachItemList
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
			Temp = newNode;//c: �NTemp���ܸ��I
			newNode->parent = father;
		}
		else
		{

			//b:�NTemp ���ܸ��I 
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

	//�p��ӧOItem�b�Ӿ𤤩Ҧ�Node��value�`�M
	bucket sumOfItemValue(100000);
	int itemValueSum = 0;


	//�q��e�j�餤item��mList�� �D�X���ݪ��C�@Node �@�@�B�z(����)
	list<Node*>::iterator it_List = item.mList.begin();
	for (; it_List != item.mList.end(); it_List++)
	{

		Node* bottonNode = *it_List;//���ɫe�̩��U��Node 
		Node* curNode = bottonNode;
		buildingListArr[index_ListArr].value = bottonNode->value;
		itemValueSum += bottonNode->value;

		//�}�l����
		while (1)
		{
			curNode = curNode->parent;
			if (curNode == NodePool.get())break;

			//�N�ȥ[��s��VaildItem
			if (sumOfItemValue.arr[curNode->itemId] != -1)
			{
				sumOfItemValue.arr[curNode->itemId] += bottonNode->value;
			}
			else
			{
				sumOfItemValue.insert(curNode->itemId, bottonNode->value);
			}

			//�ͦ��ؾ��C�s

			buildingListArr[index_ListArr].mList.push_front(curNode->itemId);

		}
		//�p��Ӧ����ɪ����� �ç�X�𦸪��̤j��
		++index_ListArr;

	}
	if (itemValueSum < MinSup)return -1;
	//�������ؾ��C�s�������X��item
	for (int index = 0; index < index_ListArr; index++)
	{
		list<int> ::iterator it_List = buildingListArr[index].mList.begin();
		for (; it_List != buildingListArr[index].mList.end();)
			//�Y��item��value�`�M���FMinSup �R��
			if (sumOfItemValue.arr[*it_List] < MinSup)
				it_List = buildingListArr[index].mList.erase(it_List);
			else
				it_List++;
	}

	return itemValueSum;
};
void FP_Tree::Loop()
{

	//�q�i�Ϊ�Item�� �@�@�B�z
	VaildItem::reverse_iterator it_VaildItem = ItemTable.rbegin();
	for (; it_VaildItem != ItemTable.rend(); it_VaildItem++)
	{
		vector<ValueAndIntList> buildingListArr(it_VaildItem->second.mList.size());

		int itemValueSum = upScanFromItem(it_VaildItem->second, buildingListArr);

		if (itemValueSum == -1)continue;

		//�N��e���j�M����T��X

		string str1 = TreeName;
		str1 += "-";
		str1 += to_string(it_VaildItem->first);
		*pOstream << str1 << ":" << to_string(itemValueSum) << endl;


		//�إ߷s���l��
		FP_Tree mNewFP_Tree(MinSup, str1,this->pOstream);
		for (int index = 0; index < buildingListArr.size(); index++)
			mNewFP_Tree.insertNode(buildingListArr[index].mList, buildingListArr[index].value);

		mNewFP_Tree.Loop();

	}





}

void FP_Tree::DivideByLoopOnce(vector<FP_Tree*> & subFPTree_Arr,ofstream * outStream)
{
	//�q�i�Ϊ�Item�� �@�@�B�z
	VaildItem::iterator it_VaildItem = ItemTable.begin();
	for (; it_VaildItem != ItemTable.end(); it_VaildItem++)
	{
		vector<ValueAndIntList> buildingListArr(it_VaildItem->second.mList.size());

		int itemValueSum = upScanFromItem(it_VaildItem->second, buildingListArr);

		if (itemValueSum == -1)continue;
		
		//�N��e���j�M����T��X
		string str1 = TreeName;
		str1 += "-";
		str1 += to_string(it_VaildItem->first);
		if (outStream != NULL)
			*outStream << str1 << ":" << to_string(itemValueSum) << endl;
		str1 = TreeName;//�����l����Y�W

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

		
		
		//�إ߷s���l��
		FP_Tree* mNewFP_Tree = new FP_Tree(MinSup, str1,this->pOstream);
		for (int index = 0; index < buildingListArr.size(); index++)
		{
			if (buildingListArr[index].mList.size()>0)
			mNewFP_Tree->insertNode(buildingListArr[index].mList, buildingListArr[index].value);
		}
		//��J��itemsum 
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
			
			//�NnewNode���J���
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


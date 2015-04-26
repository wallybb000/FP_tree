#include "Header.h"
#include "Node.h"
#include "FP_tree.h"
void FP_Tree::insertNode(list<int> &inputList, int &value)
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
			if (Temp->childByItem.size() > 1)isStraight = false;


			//�N���I�[�JeachItemList
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
			Temp = newNode;//c: �NTemp���ܸ��I
			newNode->parent = father;
		}
		else
		{

			//b:�NTemp ���ܸ��I 
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

void FP_Tree::Loop()
{

	//�q�i�Ϊ�Item�� �@�@�B�z
	VaildItem::reverse_iterator it_VaildItem = EachItemList.rbegin();
	for (; it_VaildItem != EachItemList.rend(); it_VaildItem++)
	{
		//�إߤl�𪺦�C�P�Ӧ�C��value
		vector<list<int>> subTree_ListArr(it_VaildItem->second.mList.size());
		vector<int> subTree_Listvalue(it_VaildItem->second.mList.size());
		
		//subTree_ListArr��index
		int index_ListArr = 0;

		//�p��ӧOItem�b�Ӿ𤤩Ҧ�Node��value�`�M
		bucket sumOfItemValue(100000);
		int sumBotton = 0;


		//�q��e�j�餤item��mList�� �D�X���ݪ��C�@Node �@�@�B�z(����)
		list<Node*>::iterator it_List = it_VaildItem->second.mList.begin();
		for (; it_List != it_VaildItem->second.mList.end(); it_List++)
		{
			
			Node* bottonNode = *it_List;//���ɫe�̩��U��Node 
			Node* curNode = bottonNode;
			subTree_Listvalue[index_ListArr] = bottonNode->value;
			sumBotton += bottonNode->value;
			
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

				subTree_ListArr[index_ListArr].push_front(curNode->itemId);

			}
			//�p��Ӧ����ɪ����� �ç�X�𦸪��̤j��
			++index_ListArr;

		}

		if (sumBotton < MinSup)continue;
		//�������ؾ��C�s�������X��item
		for (int index = 0; index < index_ListArr; index++)
		{
			list<int> ::iterator it_List = subTree_ListArr[index].begin();
			for (; it_List != subTree_ListArr[index].end();)
				//�Y��item��value�`�M���FMinSup �R��
				if (sumOfItemValue.arr[*it_List] < MinSup)
					it_List = subTree_ListArr[index].erase(it_List);
				else
					it_List++;
		}

		//�N��e���j�M����T��X
		string str1 = TreeName;
		
			str1 += " - ";
			str1 += to_string(it_VaildItem->first);
			out << str1 << "  value :" << to_string(sumBotton) << endl;
		//�إ߷s���l��
		FP_Tree mNewFP_Tree(MinSup, str1);
		for (int index = 0; index < index_ListArr; index++)
		{
			mNewFP_Tree.insertNode(subTree_ListArr[index], subTree_Listvalue[index]);
		}

		

		//�Y�Ӥ������� �h�~��loop
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

string FP_Tree::Serialization()
	{
	
		string SerialString;

		SerialString = to_string(NodeAmount)+"\n";
		
		queue<Node *> NodeQueue;
		NodeQueue.push(NodePool.get());

		while (1)
		{
			if (NodeQueue.empty())break;
			
			Node * curNode = NodeQueue.front();
			NodeQueue.pop();
			
			SerialString += to_string(curNode->nodeId) + " ";
			if (curNode->parent != NULL)
				SerialString += to_string(curNode->parent->nodeId) + " ";
			else
				SerialString += to_string(-1) + " ";

			SerialString += to_string(curNode->itemId) + " ";
			SerialString += to_string(curNode->value) + "\n";

			Node::IDtoNode::iterator it_Child = curNode->childByItem.begin();
			
			for (; it_Child != curNode->childByItem.end(); it_Child++)
				NodeQueue.push(it_Child->second);

		}



			//��stack���X
		return SerialString;

		

	
	}

void FP_Tree::insertNodeFromSerial(string & SerialString)
	{

		int NodeAmount;
		stringstream ss(SerialString);
		int temp;
		ss >> temp;
		ss >> NodeAmount;
		this->setMinSup( temp);
		Node **NodePtrArr = new Node*[NodeAmount]{NULL};
		
		for (int i = 0; i < 4; i++)
		{
			int temp;
			ss >> temp;
		}

		for (int NodeIndex = 0; NodeIndex < NodeAmount; NodeIndex++)
		{
			int nodeID, parentID, value, itemID;
			Node * parentPointer;
			ss >> nodeID;
			ss >> parentID;
			ss >> itemID;
			ss >> value;
			if (parentID == -1)
				parentPointer = NodePool.get();
			else
				parentPointer = NodePtrArr[parentID];

			NodePtrArr[nodeID] = new Node(nodeID, itemID, parentPointer);
			NodePtrArr[nodeID]->value = value;

			parentPointer->insertChild(NodePtrArr[nodeID]);
			this->NodeAmount++;
			
			VaildItem::iterator it_ItemList = EachItemList.find(itemID);
			if (it_ItemList == EachItemList.end())
			{
				ValueAndList newValueAndList;
				newValueAndList.mList.push_back(NodePtrArr[nodeID]);
				newValueAndList.value = NodePtrArr[nodeID]->value;
				EachItemList.insert(pair<int, ValueAndList  >(itemID, newValueAndList));

			}
			else
			{
				EachItemList[itemID].mList.push_back(NodePtrArr[nodeID]);
				EachItemList[itemID].value += NodePtrArr[nodeID]->value;
			}



		}
		
		delete[] NodePtrArr;

	}


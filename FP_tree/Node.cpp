#include "Node.h"
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
			//NodeValueList.insert(pair<int, int >(NodeAmount, 1));//
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
			Temp = it_Next->second;
			//NodeValueList[Temp->nodeId]++;//c:將該點之value累加
			Temp->value += value;
		}
	}
};


void FP_Tree::Loop()
{
	VaildItem::reverse_iterator it_VaildItem = EachItemList.rbegin();
	for (; it_VaildItem != EachItemList.rend(); it_VaildItem++)
	{
		vector<list<int>> listArr(it_VaildItem->second.mList.size());
		vector<int> listValue(it_VaildItem->second.mList.size());
		int index_ListArr = 0;

		bucket newVaildItem(100000);

		int sumBotton = 0;

		//map<int, int> nodeValue;

		list<Node*>::iterator it_List = it_VaildItem->second.mList.begin();
		for (; it_List != it_VaildItem->second.mList.end(); it_List++)
		{

			Node* bottonNode = *it_List;
			Node* curNode = bottonNode;
			listValue[index_ListArr] = bottonNode->value;
			sumBotton += bottonNode->value;
			while (1)
			{
				curNode = curNode->parent;
				if (curNode == NodePool.get())break;

				//將值加於nodeValue
				//map<int, int>::iterator it_NodeValue = nodeValue.find(curNode->nodeId);
				//if (it_NodeValue != nodeValue.end())
				//{
				//	//有找到
				//	it_NodeValue->second += bottonNode->value;
				//	newVaildItem.arr[curNode->itemId] += bottonNode->value;

				//}
				//else
				//{
				//	//沒找到
				//	nodeValue.insert(pair<int, int>(curNode->nodeId, bottonNode->value));
				//	
				//}


				//將值加於新的VaildItem
				if (newVaildItem.arr[curNode->itemId] != -1)
				{
					newVaildItem.arr[curNode->itemId] += bottonNode->value;
				}
				else
				{
					newVaildItem.insert(curNode->itemId, bottonNode->value);
				}

				//生成建樹串列群

				listArr[index_ListArr].push_front(curNode->itemId);

			}
			//計算該次爬升的長度 並找出屢次的最大值
			++index_ListArr;

		}


		//移除掉建樹串列群中的不合格item
		for (int index = 0; index < index_ListArr; index++)
		{
			list<int> ::iterator it_List = listArr[index].begin();
			for (; it_List != listArr[index].end();)
				if (newVaildItem.arr[*it_List] < MinSup)
					it_List = listArr[index].erase(it_List);
				else
					it_List++;
		}

		//建新樹
		string str1 = TreeName;
		str1 += " - ";
		str1 += to_string(it_VaildItem->first);
		out << str1 << "  value :" << to_string(sumBotton) << endl;

		FP_Tree mNewFP_Tree(MinSup, str1);
		for (int index = 0; index < index_ListArr; index++)
		{
			mNewFP_Tree.insertNode(listArr[index], listValue[index]);
		}




		if (!mNewFP_Tree.isStraight)
		{
			mNewFP_Tree.Loop();
		}
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

				}
			}
			string str2 = str1;
			list<Node *>::iterator it_ResultList1 = resultList.begin();
			list<string>::iterator it_stringList1 = stringList.begin();
			for (; it_ResultList1 != resultList.end(); it_ResultList1++, it_stringList1++)
			{

				list<string>::iterator it_stringList2 = it_stringList1;
				list<Node *>::iterator it_ResultList2 = it_ResultList1;
				for (; it_ResultList2 != resultList.end(); it_ResultList2++, it_stringList2++)
				{
					out << str2 << " - " << *it_stringList2
						<< " value :" << to_string((*it_ResultList2)->value) << endl;

				}
				str2 += " - ";
				str2 += *it_stringList1;

			}




		}


	}





}










































/*
void FP_Tree::insertNodefromMem(int *Mem ,int len,int &value)
{
Node * Temp = NodePool.get();

for (int i = 0; i < len;i++)
{
//a: 檢查Temp當前點是否已有同一點

int itemId = Mem[i];
Node::IDtoNode::iterator it_Next = Temp->childByItem.find(itemId);


if (it_Next == Temp->childByItem.end())
{
//b: 加入input 並設定該點數量,

Node * newNode = new Node(NodeAmount, itemId, NULL);
Temp->childByItem.insert(Node::IDandNode(itemId, newNode));
newNode->value = value;
//NodeValueList.insert(pair<int, int >(NodeAmount, 1));//
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
EachItemList[itemId].value+=value;
}



Node* father = Temp;
Temp = newNode;//c: 將Temp移至該點
newNode->parent = father;
}
else
{
//b:將Temp 移至該點
Temp = it_Next->second;
//NodeValueList[Temp->nodeId]++;//c:將該點之value累加
Temp->value+=value;
}

}


}*/


static int aaa = 0;
static int bbb = 0;

//void FP_Tree::loop(map<int, int> &pre_NodeValueList, VaildItem &pre_VaildItem, list<int> cur_list)
//{


//	VaildItem::iterator it_VaildItem = pre_VaildItem.begin();
//	for (; it_VaildItem != pre_VaildItem.end(); it_VaildItem++)
//	{

//		VaildItem cur_VaildItem;
//		map<int, int> cur_NodeValueList;


//		static ValueAndList * pre_VaildItemArr[20000];
//		static ValueAndList * cur_VaildItemArr[20000];
//		memset(pre_VaildItemArr, NULL, 20000);
//		memset(cur_VaildItemArr, NULL, 20000);

//		for (VaildItem::iterator it = pre_VaildItem.begin(); it != pre_VaildItem.end(); ++it)
//			pre_VaildItemArr[it->first] = &(it->second);

//		//將當下之itemid並於標頭後,作為該item延伸出去之遞迴的標頭與紀錄當前標頭的結果
//		cur_list.push_back(it_VaildItem->first);
//		/*		
//		for (list<int>::iterator it = cur_list.begin(); it != cur_list.end(); it++)
//			out << *it << " - ";
//		out << "value:" <<pre_VaildItemArr[cur_list.back()]->value<< endl;
//		*/

//		int cur_itemId = it_VaildItem->first;
//		list<Node*>::iterator it_NodeOfItem = it_VaildItem->second.mList.begin();

//		for (; it_NodeOfItem != it_VaildItem->second.mList.end(); it_NodeOfItem++)
//		{
//			Node &curNode = **it_NodeOfItem;//目前開始往上爬的item的其中一node,會有一連串的父輩
//			Node * fatherNode = &curNode;
//			while (1){

//				fatherNode = fatherNode->parent;//往上爬升
//				if (fatherNode == NodePool.get())break;//當索引到該curNode的最終父輩(最上頂點)結束爬升

//				
//				aaa++;
//				if (pre_VaildItemArr[fatherNode->itemId] != NULL)
//				//if (pre_VaildItem.find(fatherNode->itemId) != pre_VaildItem.end())
//				{//若該點之itemid存在於上一輪的VeildItem中
//					
//					bbb++;
//					map<int, int>::iterator it_NodeValueList = cur_NodeValueList.find(fatherNode->nodeId);
//					int valueOfCurNode = pre_NodeValueList[curNode.nodeId];
//					if (it_NodeValueList != cur_NodeValueList.end())
//					{
//						//若該點已存在於當前輪的NodevalueList中
//						//在該點上累加其數字

//						//VaildItem::iterator it_cVaildItem = cur_VaildItem.find(fatherNode->itemId);
//						//it_cVaildItem->second.value += valueOfCurNode;//1
//						cur_VaildItemArr[fatherNode->itemId]->value += valueOfCurNode;//1;

//						it_NodeValueList->second += valueOfCurNode;//2
//					}
//					else
//					{
//						//在當前輪的NodeValueList中設定當前爬升起始點(curNode)在上一輪的NodeValueList值
//						cur_NodeValueList.insert(
//							pair<int, int>(fatherNode->nodeId, valueOfCurNode));//1

//						//若不存在(存在於NOdeValueList 必然存在於 veildItem 若否則須判斷是否已有同樣item已先建立)

//						//VaildItem::iterator it_cVaildItem = cur_VaildItem.find(fatherNode->itemId);
//						//if (it_cVaildItem != cur_VaildItem.end())
//						ValueAndList* it_cVaildItem = cur_VaildItemArr[fatherNode->itemId];
//						if (it_cVaildItem != NULL)
//						{

//							//若該點已存在於當前輪的veildItem中
//							//
//							//it_cVaildItem->second.mList.push_back(fatherNode);
//							//it_cVaildItem->second.value += valueOfCurNode;//2
//							it_cVaildItem->mList.push_back(fatherNode);
//							it_cVaildItem->value += valueOfCurNode;//2
//						}
//						else
//						{
//							//將該點建立於NodeValueList與當前輪的vaildItem 
//							VaildItem::iterator it2_cVaildItem = cur_VaildItem.insert(
//								pair<int, ValueAndList >(fatherNode->itemId, ValueAndList()))
//								.first;

//							it2_cVaildItem->second.mList.push_back(fatherNode);
//							it2_cVaildItem->second.value = valueOfCurNode;//2

//							cur_VaildItemArr[fatherNode->itemId] = &(it2_cVaildItem->second);



//						}
//					}


//				}


//			}



//		}
//		//刪除當前各node總和值未達Minsup的item
//		for (VaildItem::iterator it3_cVaildItem = cur_VaildItem.begin(); it3_cVaildItem != cur_VaildItem.end();)
//			if (it3_cVaildItem->second.value > MinSup)
//				it3_cVaildItem++;
//			else
//				cur_VaildItem.erase(it3_cVaildItem++);




//		//進入新遞迴
//		loop(cur_NodeValueList, cur_VaildItem, cur_list);

//		//排出

//		cur_list.pop_back();
//	}


//}
/*
void FP_Tree::loop2()
{


VaildItem::reverse_iterator it_VaildItem = EachItemList.rbegin();
for (; it_VaildItem != EachItemList.rend(); it_VaildItem++)
{
vector<list<int>> listArr(it_VaildItem->second.mList.size());
vector<int> listValue(it_VaildItem->second.mList.size());
int index_ListArr = 0;

bucket newVaildItem(100000);

int sumBotton = 0;

//map<int, int> nodeValue;

list<Node*>::iterator it_List = it_VaildItem->second.mList.begin();
for (; it_List != it_VaildItem->second.mList.end(); it_List++)
{

Node* bottonNode = *it_List;
Node* curNode = bottonNode;
listValue[index_ListArr]= bottonNode->value;
sumBotton += bottonNode->value;
while (1)
{
curNode = curNode->parent;
if (curNode == NodePool.get())break;

//將值加於nodeValue
//map<int, int>::iterator it_NodeValue = nodeValue.find(curNode->nodeId);
//if (it_NodeValue != nodeValue.end())
//{
//	//有找到
//	it_NodeValue->second += bottonNode->value;
//	newVaildItem.arr[curNode->itemId] += bottonNode->value;

//}
//else
//{
//	//沒找到
//	nodeValue.insert(pair<int, int>(curNode->nodeId, bottonNode->value));
//
//}


//將值加於新的VaildItem
if (newVaildItem.arr[curNode->itemId] != -1)
{
newVaildItem.arr[curNode->itemId] += bottonNode->value;
}
else
{
newVaildItem.insert(curNode->itemId, bottonNode->value);
}

//生成建樹串列群

listArr[index_ListArr].push_front(curNode->itemId);

}
//計算該次爬升的長度 並找出屢次的最大值
++index_ListArr;

}


//移除掉建樹串列群中的不合格item
for (int index = 0; index < index_ListArr; index++)
{
list<int> ::iterator it_List = listArr[index].begin();
for (; it_List != listArr[index].end();)
if (newVaildItem.arr[*it_List] < MinSup)
it_List = listArr[index].erase(it_List);
else
it_List++;
}

//建新樹
string str1= TreeName;
str1 += " - ";
str1 += to_string(it_VaildItem->first);
out << str1<<"  value :" << to_string(sumBotton)<<endl;

FP_Tree mNewFP_Tree(MinSup,str1);
for (int index = 0; index < index_ListArr; index++)
{
mNewFP_Tree.insertNode(listArr[index],listValue[index]);
}




if (!mNewFP_Tree.isStraight)
{
mNewFP_Tree.loop2();
}
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

}
}
string str2 = str1;
list<Node *>::iterator it_ResultList1 =resultList.begin();
list<string>::iterator it_stringList1 = stringList.begin();
for (; it_ResultList1 != resultList.end(); it_ResultList1++,it_stringList1++)
{

list<string>::iterator it_stringList2 = it_stringList1;
list<Node *>::iterator it_ResultList2 = it_ResultList1;
for (; it_ResultList2 != resultList.end(); it_ResultList2++,it_stringList2++)
{
out << str2 << " - " << *it_stringList2
<< " value :" << to_string((*it_ResultList2)->value)<<endl;

}
str2 += " - ";
str2 += *it_stringList1;

}




}


}




}
*/

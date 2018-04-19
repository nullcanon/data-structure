#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

#define DateType int

typedef struct Node
{
	DateType data;
	struct Node* next;
}Node,*pNode;

void InitList(pNode *head)//初始化
{
	assert(head);

	(*head) = NULL;
}

pNode BuyNode(DateType e)//申请新节点
{
	pNode lk = (pNode)malloc(sizeof(Node));
	if(!lk)
		return NULL;

	lk->data = e;
	return lk;
}
void PushBack(pNode *head,DateType e)//尾插
{
	pNode nd,tmp;
	assert(head);
	nd = BuyNode(e);
	if(!nd)
		return;
	else if(*head == NULL)
	{
		*head = nd;
		nd->next = NULL;
	}
	else
	{
		tmp = *head;
		while(tmp->next)
		{
			tmp = tmp->next;
		}
		tmp->next = nd;
		nd->next = NULL;
	}
				
}

void PopBack(pNode *head)//尾删
{
	pNode tmp;
	pNode q = NULL;
	assert(head);

	if(*head == NULL)
		return;

	tmp = *head;
	while(tmp->next)
	{
		q = tmp;
		tmp = tmp->next;
	}
	if(q != NULL)//多个节点
	{
		q->next = NULL;
		free(tmp);
	}
	else//一个节点
	{
		free(tmp);		
		*head = NULL;
	}
}

void PrintList(pNode head)//打印链表
{
	pNode tmp = head;
	while(tmp)
	{
		printf("%d-->",tmp->data);
		tmp = tmp->next;
	}
	printf("NULL\n");
}

void PushFront(pNode *head,DateType e)//头插
{
	pNode nd = BuyNode(e);
	assert(head);
	if(nd)
	{
		nd->next = *head;
		*head = nd;
	}
}

void PopFront(pNode *head)//头删
{
	pNode tmp;
	assert(head);
	if(!*head)
		return;
	tmp = *head;
	*head = (*head)->next;
	free(tmp);
}

pNode Find(pNode head,DateType e)//找到指定元素的位置并返回,没找到返回NULL
{
	pNode tmp;
	assert(head);

	tmp = head;
	while(tmp->data != e)
	{
		tmp = tmp->next;
		if(tmp == NULL)
			return NULL;
	}
	return tmp;
}
void Inster(pNode *head,pNode pos,DateType e)//指定位置插入
{
	pNode nd;
	assert(head);
	if(pos && *head)
	{
		nd = BuyNode(e);
		nd->next = pos->next;
		pos->next = nd;
	}
}

void Erase(pNode *head,pNode pos)//指定位置删除
{
	pNode tmp;
	pNode q = NULL;//q是记录tmp的前一个位置
	assert(head);
	if(*head && pos)
	{
		tmp = *head;
		while(tmp != pos)
		{
			q = tmp;
			tmp = tmp->next;
		}
		if(q != NULL)
		{
			q->next = tmp->next;
			free(tmp);
		}
		else
		{
			free(tmp);		
			*head = NULL;
		}
	}
}

int LenList(pNode head)//链表长度
{
	int count = 0;
	pNode tmp = head;
	while(tmp)
	{
		tmp = tmp->next;
		++count;
	}
	return count;
}

void Destroy(pNode *head)//销毁链表
{
	pNode tmp = *head;
	while(tmp)
	{
		pNode del = tmp;
		tmp = tmp->next;
		free(del);
	}
	*head = NULL;
}
/************************************
*
*2. 从尾到头打印单链表
*3. 删除一个无头单链表的非尾节点（不能遍历链表）
*4. 在无头单链表的一个非头节点前插入一个节点（不能遍历链表）
*5. 单链表实现约瑟夫环(JosephCircle)
*6. 逆置/反转单链表
*9. 查找单链表的中间节点，要求只能遍历一次链表
*
*************************************/
void PrintListTail2Head(pNode head)//倒着打印
{
	if(head)
	{
		PrintListTail2Head(head->next);
		printf("%d-->",head->data);
	}
}

void DelNotTailNode(pNode head,pNode pos)//不遍历删除非尾节点
{
	pNode del;
	if(!head || !pos || !pos->next)
		return;
	del = pos->next;
	pos->data = del->data;
	pos->next = del->next;
	free(del);
}

void InsterNotErg(pNode *head,pNode pos,DateType e)//不遍历在结点前插入
{
	pNode nd;
	assert(head);
	if(!pos)
		return;
	nd = BuyNode(pos->data);
	if(nd)
	{
		pos->data = e;
		nd->next = pos->next;
		pos->next = nd;
	}
}

pNode DeathRing(pNode *head,int m)//约瑟夫环
{
	int tmp;
	pNode del,cur;
	assert(head);
	if((*head)->next == NULL)
		return(*head);

	//构环
	cur= *head;
	while(cur->next)
		cur = cur->next;
	cur->next = *head;

	cur = *head;
	while(cur->next != cur)
	{
		//报数
		tmp = m;
		while(--tmp)
		{
			cur = cur->next;
		}
		//删除
		del = cur->next;
		cur->data = del->data;
		cur->next = del->next;
		free(del);
		*head = cur;
	}
	//解环
	cur->next = NULL;
	*head = cur;
	return (*head);
}

pNode FindMidNode(pNode head)//返回中间结点
{
	pNode fast,low;
	assert(head);
	if(!head->next)
		return head;

	fast = head,low = head;
	while(fast && fast->next)
	{
		low = low->next;
		fast = fast->next->next;
	}
	return low;
}

pNode ReverseList(pNode *head)//非递归逆转
{
	pNode first,second,third;
	assert(head);
	if(!(*head)->next)
		return (*head);
	first = *head;
	second = first->next;
	third = second->next;
	
	first->next = NULL;
	while(third)
	{
		second->next = first;

		first = second;
		second = third;
		third = third->next;
	}
	second->next = first;
	*head = second;
	return (*head);
}

//
/************测试代码***************/
void test5()
{
	pNode list;
	InitList(&list);

	PushBack(&list,1);
	PushBack(&list,2);
	PushBack(&list,3);
	PushBack(&list,4);
	PushBack(&list,5);
	PushBack(&list,6);
	ReverseList(&list);
	PrintList(list);

}

void test4()
{
	pNode list;
	InitList(&list);

	PushBack(&list,1);
	PushBack(&list,2);
	PushBack(&list,3);
	PushBack(&list,4);
	PushBack(&list,5);
	PushBack(&list,6);
	
	DeathRing(&list,3);
	PrintList(list);


}

void test3()
{
	pNode list;
	pNode pos;
	InitList(&list);

	PushBack(&list,1);//1->NULL
	PushBack(&list,2);//1->2->NULL
	PushBack(&list,3);//1->2->3->NULL
	PushFront(&list,0);//0->1->2->3->NULL

	pos = Find(list,2);
	DelNotTailNode(list,pos);//0->1->3->NULL
	PrintList(list);

	pos = Find(list,3);
	InsterNotErg(&list,pos,2);//0->1->2->3->NULL
	PrintList(list);
}

void test2()
{
	pNode list;
	InitList(&list);

	PushBack(&list,1);//1->NULL
	PushBack(&list,2);//1->2->NULL
	PushBack(&list,3);//1->2->3->NULL
	PushFront(&list,0);//0->1->2->3->NULL

	PrintListTail2Head(list);

}

/*
// 使用冒泡对单链表进行排序 
void BubbleSort(PNode pHead) 

// 合并两个已序链表，合并之后新链表依然有序 
PNode MergeList(PNode pHead1, PNode pHead2) 

// 查找无头单链表的倒数第K个结点 
PNode FindLastKNode(PNode pHead, int K)； 

// 删除无头单链表的倒数第K个结点 
PNode DeleteLastKNode(PNode pHead, int K)； 

// 判断链表是否带环，如果带环求环的长度，并给出入口点 
PNode HasCircle(PNode pHead)； 
int GetCircleLen(PNode pHead)； 
PNode GetEnterNode(PNode pHead, PNode pMeetNode)；
*/

void BubbleListSort(pNode head)
{
	int flag;
	pNode mark,Cur,nextCur;
	if(NULL == head->next)
		return;
	mark = NULL;

	while(mark != head)
	{
		flag = 0;
		Cur = head;
		nextCur = Cur->next;
		while(nextCur != mark)
		{
			if(Cur->data > nextCur->data)
			{
				DateType tmp = Cur->data;
				Cur->data = nextCur->data;
				nextCur->data = tmp;
				flag = 1;
			}
			Cur = Cur->next;
			nextCur = nextCur->next;
		}
		mark = Cur;

		if(!flag)
			break;
	}
}

pNode MargeTwoOrderList(pNode head1,pNode head2)
{
	pNode Cur1=head1,Cur2=head2,New=NULL;
	if(NULL == head1)
		return head2;
	if(NULL == head2)
		return head1;
	if(!head1 && !head2)
		return NULL;

	if(Cur1->data > Cur2->data)
	{
		New = Cur2;
		Cur2 = Cur2->next;
	}
	else
	{
		New = Cur1;
		Cur1 = Cur1->next;
	}
	head1 = New;
	while(Cur1 && Cur2)
	{
		if(Cur1->data > Cur2->data)
		{
			New->next = Cur2;
			New = New->next;
			Cur2 = Cur2->next;

		}
		else
		{
			New->next = Cur1;
			New = New->next;
			Cur1 = Cur1->next;
		}
	}
	if(NULL == Cur1)
		New->next = Cur2;
	if(NULL == Cur2)
		New->next = Cur1;

	return head1;
}

pNode FindLastKNode(pNode head,int k)
{
	pNode pFast = head,pLow = head;
	if(NULL == head || k<=0)
		return NULL;
	while(k--)
	{
		pFast = pFast->next;
	}
	while(pFast != NULL)
	{
		pFast = pFast->next;
		pLow = pLow->next;
	}
	return pLow;
}

void DelLastKNode(pNode *head,int k)
{
	pNode kNode,pDel = NULL,pCur = *head,pMark = NULL;
	kNode = FindLastKNode(*head,k);
	if(NULL == kNode)
		return;
	if(NULL == kNode->next)
	{
		while(pCur != kNode)
		{
			pMark = pCur;
			pCur = pCur->next;
		}
		pMark->next = NULL;
		free(kNode);
	}
	else
	{
		pDel = kNode->next;
		kNode->data = pDel->data;
		kNode->next = pDel->next;
		free(pDel);
	}
}

pNode HasCircle(pNode head)
{
	pNode pFast=head,pLow=head;
	while(pFast && pFast->next)
	{
		if(pLow == pFast)
			return pLow;
		pFast = pFast->next->next;
		pLow = pLow->next;
	}
	return NULL;
}
int GetCircleLen(pNode head)
{
	int count = 0;
	pNode pMeet = HasCircle(head);
	pNode pCur = pMeet;
	if(pMeet == NULL)
		return 0;

	while(pMeet != pCur)
	{
		pCur = pCur->next;
		count++;
	}
	return count;
}
pNode GetEnterNode(pNode head, pNode pMeet)
{
	pNode pCur,pMeetCur;
	if(!pMeet || !head)
		return NULL;
	pCur = head;
	pMeetCur = pMeet;
	while(pCur != pMeetCur)
	{
		pCur = pCur->next;
		pMeetCur = pMeetCur->next;
	}
	return pCur;
}





//pNode ReverseList2(pNode *head)//递归逆转
//{
//	pNode ret;
//	if(!*head || !(*head)->next)
//		return *head;
//	else
//	{
//		ret = ReverseList2(&(*head)->next);
//		(*head)->next->next = *head;
//		(*head)->next = NULL;
//		return ret;
//	}
//}




int ListHasInterest(pNode head1,pNode head2)
{
	pNode pCur1 = head1,pCur2 = head2;
	if(!head1 || !head2)
		return 0;

	while(pCur1->next)
		pCur1 = pCur1->next;
	while(pCur2->next)
		pCur2 = pCur2->next; 

	if(pCur1 == pCur2)
		return 1;
	return 0;
}

pNode ListInterestion(pNode head1,pNode head2)//求交点
{
	pNode pCur1=head1,pCur2=head2;
	int d_value = LenList(head1)-LenList(head2);
	if(!ListHasInterest(head1,head2))
		return NULL;

	if(d_value > 0)
	{
		while(d_value--)
			pCur1 = pCur1->next;
	}
	else
	{
		while(d_value++)
			pCur2 = pCur2->next;
	}
	while(pCur1->next)
	{
		if(pCur1 == pCur2)
			return pCur1;
		pCur1 = pCur1->next;
		pCur2 = pCur2->next;
	}
	return NULL;
}


//int main()
//{
//	pNode list1,list2;
//
//	InitList(list1);
//	PushBack(list1,1);
//	PushBack(list1,2);
//	PushBack(list1,3);
//	PushBack(list1,4);
//	PushBack(list1,5);
//
//	InitList(list2);
//	PushBack(list2,1);
//	PushBack(list2,2);
//	PushBack(list2,3);
//	PushBack(list2,4);
//	PushBack(list2,5);
//
//	FindList()
//
//	return 0;
//}

#include <types.h>
typedef int ElementType;
#include <queue.h>
#include <stdio.h>
int IsEmptyQueue(pt_Queue q)
{
	return (q->Size == 0);
}
int IsFullQueue(pt_Queue q)
{
	return (q->Size == q->Capacity);
}

int CreateQueue(pt_Queue q,int MaxElements,ElementType *array)
{
	if((MaxElements < 1) || (array == NULL) || (q == NULL))
		return -1;
	q->Size = 0;
	q->Capacity = MaxElements;
	q->Front = 0;
	q->Rear = 0;
	q->Array = array;
	return 0;
}
int DisposeQueue(pt_Queue q)
{
	int num = 0; 
	if(!q)
	{
		num = q->Capacity - q->Size;
	}
	return num;
}
void MakeEmptyQueue(pt_Queue q)
{
	if(!q)
	{
		q->Size = 0;
		q->Front = 0;
		q->Rear = 0;	
	}
}
void Enqueue(ElementType x,pt_Queue q)
{
	if(!q || IsFullQueue(q))
		return;
	q->Array[q->Rear] = x;
	q->Rear= (q->Rear + 1) % q->Capacity;
	(q->Size < q->Capacity) ? (q->Size)++ : (q->Size = q->Capacity);	
}
ElementType FrontQueue(pt_Queue q)
{
	ElementType temp = 0;
	if((q != NULL)  && !IsEmptyQueue(q))
	{
		temp = q->Array[q->Front];
		q->Front = (q->Front  + 1) % q->Capacity;
		(q->Size > 0) ? (q->Size)--:0;
	}
	return temp;
}
void Dequeue(pt_Queue q)
{
	if(!q)
	{
		q->Size = 0;
		q->Capacity = 0;
		q->Front = 0;
		q->Rear = 0;
		q->Array = NULL;
	}
}
ElementType FrontAndDequeue(pt_Queue q)
{
	ElementType temp = 0;
	if(!q)
	{
		temp = FrontQueue(q);
		Dequeue(q);
	}
	return temp;
}


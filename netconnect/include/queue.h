#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdio.h>
/*
The queue structual,you should initializing 
the queue with correct type and size.This queue
uses a empty element for full queue.
*/
typedef struct _QueueRecord{
	int Capacity;
	int Front;
	int Rear;
	int Size;
	ElementType *Array;
}t_Queue,*pt_Queue;

/*now it's the queue operation functions declaration*/
int IsEmptyQueue(pt_Queue q);
int IsFullQueue(pt_Queue q);
int CreateQueue(pt_Queue q,int MaxElements,ElementType* array);
int DisposeQueue(pt_Queue q);
void MakeEmptyQueue(pt_Queue q);
void Enqueue(ElementType x,pt_Queue q); 
ElementType FrontQueue(pt_Queue q);
void Dequeue(pt_Queue q);
ElementType FrontAndDequeue(pt_Queue q);
#endif


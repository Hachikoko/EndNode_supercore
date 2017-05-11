#ifndef _LINK_QUEUE_H_
#define _LINK_QUEUE_H_
#include "sys.h"


typedef struct node{
	u8* ptr_data;
	struct node * next;
}Queue_Node;

typedef struct link_queue{
	struct node * front;
	struct node * rear;
	u32 length;
}Link_Queue;


Link_Queue* creat_link_queue(void);
Queue_Node* creat_link_node(void);
u8 pop_node(Link_Queue* link_queue);
u8 push_back_node(Link_Queue* link_queue,u8* ptr_data);


#endif


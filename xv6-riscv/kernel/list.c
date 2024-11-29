

#include "types.h"
#include "stddef.h"
#include "list.h"
//#include "stdio.h"


int listCount = 0; /*keeps track of how many lists we have*/ 
int nodeCount = 0; /*keeps track of how many nodes have been filled*/

struct list listSpace[MAX_LISTS];

struct node nodeSpace[MAX_LISTS*MAX_NODES];

struct node removedNodes[MAX_LISTS*MAX_NODES];

struct list removedLists[MAX_LISTS];


/* Creates and returns a brand new empty list, making all values initialized 
 * to NULL
 * If there is already a value in storage, will return NULL as failure */
struct list * ListCreate()
{
	if (listCount < MAX_LISTS)
	{
		struct list * list;

		if (removedListNum > 0)
		{
			list = &listSpace[removedLists[\
			       removedListNum-1].listNum];

			removedListNum -= 1;
		}

		else
		{
			list = &listSpace[listCount]; /* grab space for list*/
			
			list->listNum = listCount;
			listCount+=1;   /* incriment number of lists for  
					   MAX_LISTS*/
		}

		list->first = NULL;	/*initializes all attributes inside
					  the new list*/
		list->last = NULL;
		list->cur = NULL;
		list->prev = NULL;
		list->size = 0;
	
		return list;
	}
	else

        	return NULL; /* fail case for if MAX_LIST is reached*/
}

/* Adds a new node directly after the cur, then makes the new item the cur
 * returns 0 on success and -1 on failure */
int ListAdd(struct list* list, void * item) 
{
	if (list->size < MAX_NODES)
	{	
		struct node * node;
		if (removedNodeNum > 0)
		{
			node = &nodeSpace[removedNodes[removedNodeNum-1]\
				.nodeNum];
			removedNodeNum-=1;
		}
		
		else
		{
			node = &nodeSpace[nodeCount];
			node->nodeNum = nodeCount;
			nodeCount+=1;
		}

		node->item = item;

		if (list->size == 0)	/* Case for empty list*/
		{
			node->next = NULL;
			node->before = NULL;
			list->cur = node;
			list->first = node;
			list->last = node;
		}
		
		else if (list->cur == NULL && list->prev == NULL) 
		/* Before first case*/
		{
			node->next = list->first;
			node->before = NULL;
			list->cur = node;
			list->first->before = node;
			list->first = node;
		}
		
		else if ((list->cur == NULL && list->prev != NULL) || list->cur\
				== list->last)
		/* Last and After last Case*/
		{
			node->next = NULL;
			node->before = list->last;
			list->last->next = node;
			list->cur = node;
			list->last = node;
			list->prev = node->before;
		}

		else	/* Anything other than the cases above*/
		{
			node->next = list->cur->next;
			list->cur->next = node;
			node->before = list->cur;
			node->next->before = node;

			list->cur = node;
			list->prev = node->before;
		}
		list->size+=1;
		nodeSpace[node->nodeNum] = *node;
		listSpace[list->listNum] = *list; /*return pointer to the space*/
		return 0;
	}
	else	/* if the list has max nodes, fail*/
		return -1;

}


/* Adds a new node infront of the first node, then makes the new item the cur
 * returns 0 on success and -1 on failure */
int ListPrepend(struct list* list, void * item)
{
        if (list->size < MAX_NODES)
        {
                if (list->size == 0)     /* Case for empty list*/
                {
                        return ListAdd(list,item);
                }

                else
                {

                        struct node * node;
			if (removedNodeNum > 0)
			{
				node = &nodeSpace[removedNodes[\
					removedNodeNum-1].nodeNum];

				removedNodeNum-=1;
			}

			else
			{
				node = &nodeSpace[nodeCount];
				node->nodeNum = nodeCount;
				nodeCount+=1;
			}

			node->item = item;

			node->next = list->first;

			list->first->before = node;

			node->before = NULL;

			list->first = node;

			list->cur = node;

			list->prev = NULL;

                        list->size+=1;
			nodeSpace[node->nodeNum] = *node;
                        listSpace[list->listNum] = *list; /*return pointer to
                                                          the space*/
                        return 0;
                }
        }
        else
                return -1; /* if the list has max nodes, fail*/
}



/* Adds a new node after the last node, then makes the new item the cur
 * returns 0 on success and -1 on failure */
int ListAppend(struct list* list, void * item)
{
        if (list->size < MAX_NODES)
        {
                if (list->size == 0)     /* Case for empty list*/
                {
                        return ListAdd(list,item);
                }

                else
                {

                        struct node * node;
			if (removedNodeNum > 0)
			{
				node = &nodeSpace[removedNodes[\
				       removedNodeNum-1].nodeNum];

				removedNodeNum-=1;
			}
	
			else
			{
				node = &nodeSpace[nodeCount];
				node->nodeNum = nodeCount;
				nodeCount+=1;
			}
			
			node->item = item;

                        node->next = NULL;

                        list->last->next = node;

                        node->before = list->last;

                        list->last = node;

			list->cur = node;

                        list->prev = node->before;

                        list->size+=1;
			nodeSpace[node->nodeNum] = *node;
                        listSpace[list->listNum] = *list; /*return pointer to
                                                          the space*/
                        return 0;
                }
        }
        else
                return -1; /* if the list has max nodes, fail*/

} 


/* REMOVERS */


int removedNodeNum = 0;
int removedListNum = 0;

/* Returns the current item and removes it, makes the next item cur*/
void * ListRemove(struct list* list)
{
	if (list->cur == NULL)
		return NULL;
	
	else
	{
		removedNodes[removedNodeNum] = *(list->cur);
		removedNodeNum += 1;
		list->cur->before = NULL;

		if (list->cur == list->first)
			list->first = list->cur->next;
		
		list->cur = list->cur->next;
		if (list->cur != NULL)
		{
			list->cur->before->next = NULL;
			list->cur->before = list->prev;
		}
		
		else if (list->cur == NULL)
			list->last = list->prev;
		
		if (list->prev != NULL)
			list->prev->next = list->cur;
		
		list->size -= 1; 
		listSpace[list->listNum] = *list;

		if (list->cur == NULL)
			return NULL;
		return list->cur->item;
	}
}

/* deletes the list */
void ListFree(struct list* list, void (*itemFree)(void * itemToBeFreed))
{
	ListFirst(list);
	list = &listSpace[list->listNum];
	while (list->first != NULL)
	{
		itemFree(list->first->item);
		ListRemove(list);
		list = &listSpace[list->listNum];
	}
	removedLists[removedListNum] = *list;
	removedListNum += 1;

}

/* return the last item and remove it from the list */
void * ListTrim(struct list* list)
{
	if (list->size == 0)
		return NULL;
	else
	{
		removedNodes[removedNodeNum] = *(list->last);
		removedNodeNum += 1;
		list->cur = list->last->before;
		list->last = list->last->before;
		if (list->size != 1)
		{
			list->prev = list->cur->before;
			list->cur->next->before = NULL;
			list->cur->next = NULL;
		}
		list->size -= 1;
		listSpace[list->listNum] = *list;

		if (list->cur == NULL)
			return NULL;
		return list->cur->item;
	}
}


/* MOVERS */

/* moves the cur to the first node in the list and returns cur */
void * ListFirst(struct list* list)
{
	if (list->size != 0)
	{
        	list->cur = list->first;
		list->prev = NULL;
		listSpace[list->listNum] = *list;
		return list->cur->item;
	}
	else
		return NULL;
}

/* moves the cur to the last node in the list and returns cur */
void * ListLast(struct list* list)
{
	if (list->size != 0)
	{
        	list->cur = list->last;
		list->prev = list->last->before;
		listSpace[list->listNum] = *list;
		return list->cur->item;
	}
	else
		return NULL;
}

/* moves the cur and prev to their next nodes in the list and returns cur */
void * ListNext(struct list* list)
{
	if (list->cur == NULL && list->prev == NULL)
	{
		list->cur = list->first;
		listSpace[list->listNum] = *list;
		return list->cur->item;
	}

	else if (list->cur == NULL)
	{
		return NULL;
	}

	else
	{
		list->cur = list->cur->next;
		if (list->prev ==NULL)
			list->prev = list->cur->before;
		else
			list->prev = list->prev->next;
		listSpace[list->listNum] = *list;
		
		if (list->cur == NULL)
			return NULL;
		return list->cur->item;
	}
}

/* moves the cur and prev to their previous nodes in the list and returns cur*/
void * ListPrev(struct list* list)
{
	if (list->cur == NULL && list->prev == NULL)
		return NULL;

	else if (list->cur == NULL)
	{
		list->prev = list->prev->before;
		list->cur = list->last;
		listSpace[list->listNum] = *list;
		return list->cur->item;
	}

	else
	{
		list->prev = list->prev->before;
		list->cur = list->cur->before;
		listSpace[list->listNum] = *list;

		if (list->cur ==NULL)
			return NULL;
		return list->cur->item;
	}
}

/* returns the number of items in the list */
int ListCount(struct list* list)
{
        return list->size;
}

/* returns the cur in list */
void * ListCurr(struct list* list)
{
	if (list->cur == NULL)
		return NULL;
        return list->cur->item;
}



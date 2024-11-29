#ifndef LIST_H
#define LIST_H

#define MAX_NODES 200

#define MAX_LISTS 2

struct node
{
        void * item; /* value in the node*/

        struct node * next;  /* pointer to the next node */

        struct node * before;  /* pointer to the previous node */

	int nodeNum; /* the node number signifies where in nodeSpace it is */
};

struct list
{

        struct node * first; /* first node in the list of nodes*/

        struct node * last; /* last node in the list of nodes*/

        struct node * cur; /* the pointer to the node that the list is
                              currently on*/

        struct node * prev; /* the node before the current node*/

        int size; /* lengeth of list */

	int listNum; /*the list number that this list is (first list,
		       second list etc)*/
};



extern struct list listSpace[MAX_LISTS]; 

extern struct node nodeSpace[MAX_LISTS*MAX_NODES];

extern struct node removedNodes[MAX_LISTS*MAX_NODES];

extern struct list removedLists[MAX_LISTS];




extern int removedNodeNum;

extern int removedListNum;

struct list * ListCreate();

int ListCount(struct list* list);

void * ListCurr(struct list* list);

void * ListFirst(struct list* list);

void * ListLast(struct list* list);

void * ListNext(struct list* list);

void * ListPrev(struct list* list);




int ListAdd(struct list* list, void * item);

int ListInsert(struct list* list, void * item);

int ListPrepend(struct list* list, void * item);

int ListAppend(struct list* list, void * item);

void * ListRemove(struct list* list);

void ListFree(struct list* list, void (*itemfree)(void * itemToBeFreed));

void * ListTrim(struct list* list);

#endif


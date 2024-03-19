#ifndef C_ALGORITHM_H
#define C_ALGORITHM_H


#define ABS(x)	(x < 0)?(-(x)):(x)


/* Linked List ADT */

struct LNode {
	void* data;
	struct LNode* next;
};

struct WLList {
	int count;
	struct LNode* head;
	struct LNode* tail;

	/* Pointers to User defined functions */
	int (*CMP)(const void* x, const void* y);
	void* (*CTOR)(void* x);
	void (*DTOR)(void* x);
};

struct WLList* WCreateList(int (*CMP)(const void* x, const void* y),\
													 void* (*CTOR)(void* x),\
													 void (*DTOR)(void* x));
int WAddToList(struct WLList* l, void* data);
void WDeleteList(struct WLList* l);
void* WFindInList(struct WLList* l, void* key);
int WDeleteFromList(struct WLList* l, void* key);
int WInsertToList(struct WLList* l, void* key, void *data);
int WPrependToList(struct WLList* l, void* data);
int WAppendToList(struct WLList* l, void* data);

#endif /* C_ALGORITHM_H */
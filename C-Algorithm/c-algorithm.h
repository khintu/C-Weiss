#ifndef C_ALGORITHM_H
#define C_ALGORITHM_H


#define ABS(x)	(x < 0)?(-(x)):(x)
#define FALSE	0
#define TRUE	1

/* ---Linked List ADT--- */

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
void WIteratorList(struct WLList* l, void (*ITR)(void*));
void WSortList(struct WLList* l);


/* ---Doubly Linked List ADT--- */

struct DLNode {
	void* data;
	struct DLNode* prev;
	struct DLNode* next;
};

struct WDLList {
	int count;
	struct DLNode* head;
	struct DLNode* tail;

	/* Pointers to User defined functions */
	int (*CMP)(const void* x, const void* y);
	void* (*CTOR)(void* x);
	void (*DTOR)(void* x);
};

struct WDLList* WCreateDList(int (*CMP)(const void* x, const void* y), \
														void* (*CTOR)(void* x), \
														void (*DTOR)(void* x));
int WAddToDList(struct WDLList* l, void* data);
void WDeleteDList(struct WDLList* l);
void* WFindInDList(struct WDLList* l, void* key);
int WDeleteFromDList(struct WDLList* l, void* key);
int WInsertToDList(struct WDLList* l, void* key, void* data);
int WPrependToDList(struct WDLList* l, void* data);
int WAppendToDList(struct WDLList* l, void* data);
void WIteratorDList(struct WDLList* l, void (*ITR)(void*));
void WRevIteratorDList(struct WDLList* l, void (*ITR)(void*));
void WSortDList(struct WDLList* l);

#endif /* C_ALGORITHM_H */
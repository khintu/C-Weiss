#ifndef C_ALGORITHM_H
#define C_ALGORITHM_H

/* C algorithms on Lists, Sets & Graphs */

#define ABS(x)	((x) < 0)?(-(x)):(x)
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))

#define FALSE	0
#define TRUE	1
#define NUL	((char)'\0')

typedef int (*WCMPFP)(const void* x, const void* y);

/* ---Linked List ADT--- */

struct LNode {
	void* data;
	struct LNode* next;
};

struct WLList {
	unsigned count;
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
int WInsertToSortdList(struct WLList* l, void* data);

/* ---Circular Linked List---*/

struct WCLList {
	unsigned count;
	struct LNode* head;
	struct LNode* tail;	/* Tail always points to head */

	/* Pointers to User defined functions */
	int (*CMP)(const void* x, const void* y);
	void* (*CTOR)(void* x);
	void (*DTOR)(void* x);
};

struct WCLList* WCreateCList(int (*CMP)(const void* x, const void* y), \
														void* (*CTOR)(void* x), \
														void (*DTOR)(void* x));
int WAddToCList(struct WCLList* l, void* data);
void WDeleteCList(struct WCLList* l);
void* WFindInCList(struct WCLList* l, void* key);
int WDeleteFromCList(struct WCLList* l, void* key);
int WInsertToCList(struct WCLList* l, void* key, void* data);
int WPrependToCList(struct WCLList* l, void* data);
int WAppendToCList(struct WCLList* l, void* data);
void WIteratorCList(struct WCLList* l, void (*ITR)(void*), int Cycle);
void WSortCList(struct WCLList* l);
int WInsertToSortdCList(struct WCLList* l, void* data);

/* ---Doubly Linked List ADT--- */

struct DLNode {
	void* data;
	struct DLNode* prev;
	struct DLNode* next;
};

struct WDLList {
	unsigned count;
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
int WInsertToSortdDList(struct WDLList* l, void* data);
void WQuickSortDList(struct WDLList* dll);

/* ---Stack ADT out of Linked List--- */

 /* Let the head of the list be the entry/exit of stack */
struct WLStack {
	struct WLList *ll;
};

struct WLStack* WCreateLStack(int (*CMP)(const void* x, const void* y), \
														 void* (*CTOR)(void* x), \
														 void (*DTOR)(void* x));
void WDeleteLStack(struct WLStack* stk);
int WIsEmptyLStack(struct WLStack* stk);
unsigned WSizeOfLStack(struct WLStack* stk);
void* WTopLStack(struct WLStack* stk);
int WPushLStack(struct WLStack* stk, void* key);
void* WPopLStack(struct WLStack* stk);

/* ---Queue ADT out of Linked List--- */

 /* Let the head of the list be the exit of queue and tail be the entry */
struct WLQueue {
	struct WLList* ll;
};

struct WLQueue* WCreateLQueue(int (*CMP)(const void* x, const void* y), \
															void* (*CTOR)(void* x), \
															void (*DTOR)(void* x));
void WDeleteLQueue(struct WLQueue* que);
int WIsEmptyLQueue(struct WLQueue* que);
unsigned WSizeOfLQueue(struct WLQueue* que);
void* WHeadLQueue(struct WLQueue* que);
void* WTailLQueue(struct WLQueue* que);
int WEnqueueLQueue(struct WLQueue* que, void* key);
void* WDequeueLQueue(struct WLQueue* que);

/* ---Priority Queue ADT out of Linked List--- */

 /* Let the head of the list be the exit of queue and tail be the entry,
	 And the queue stays sorted with each enQ/deQ operation in increasing 
	 order. */
struct WPLQueue {
	struct WDLList* dll;
};

struct WPLQueue* WCreatePLQueue(int (*CMP)(const void* x, const void* y), \
															void* (*CTOR)(void* x), \
															void (*DTOR)(void* x));
void WDeletePLQueue(struct WPLQueue* que);
int WIsEmptyPLQueue(struct WPLQueue* que);
unsigned WSizeOfPLQueue(struct WPLQueue* que);
void* WHeadPLQueue(struct WPLQueue* que);
void* WTailPLQueue(struct WPLQueue* que);
int WEnqueuePLQueue(struct WPLQueue* que, void* key);
void* WDequeuePLQueue(struct WPLQueue* que);

/* ---Priority Queue ADT on Arrays(Binary Heap)--- */

struct WPAQueue {
	int heapSize; /* MaxHeap tree length */
	int Length; /* Array length */
	void** array;  /* dynamically created array of void* for heap */

	int (*CMP)(const void* x, const void* y);
	void* (*CTOR)(void* x);
	void (*DTOR)(void* x);
};

struct WPAQueue* WCreatePAQueue(int Len,\
																int (*CMP)(const void* x, const void* y), \
																void* (*CTOR)(void* x), \
																void (*DTOR)(void* x));
void WDeletePAQueue(struct WPAQueue* pQ);
void* WMaximumPAQueue(struct WPAQueue* pQ);
void* WHeapExtractMaxPAQueue(struct WPAQueue* pQ);
int WHeapIncKeyPAQueue(struct WPAQueue* pQ, int idx, void* key);
int WMaxHeapInsertPAQueue(struct WPAQueue* pQ, void* key);

/* ---Hash Map/Dictionary ADT--- */

struct ChainItem
{
	void* key;
	void* value;
	struct ChainItem* next;
};

struct WHashMap
{
	struct ChainItem** table; /* dynamically allocated array of chains */
	int tabSize;							/* Size of universal key set */
	int (*KEYCMP)(const void*, const void*);
	void* (*CTORK)(void* x); /* Constructor/Destructor for key/value */
	void (*DTORK)(void* x);
	void* (*CTORV)(void* x);
	void (*DTORV)(void* x);
	int (*HASHFN)(const void*); /* Hash value from a hash function */
};

struct WHashMap* WCreateHashMap(int tabSize, int (*HASHFN)(const void*), \
																int (*KCMP)(const void*, const void*), \
																void* (*CTORK)(void*), \
																void (*DTORK)(void*), \
																void* (*CTORV)(void*), \
																void (*DTORV)(void*));
void WDeleteHashMap(struct WHashMap*);
void* WSearchKeyHashMap(struct WHashMap*, void *key);
int WInsertKeyValHashMap(struct WHashMap* hmap, void* key, void* val);
int WDeleteKeyHashMap(struct WHashMap* hmap, void* key);

/* ---Binary Search Tree ADT--- */

struct BSTNode
{
	void* data; /* key ADT */
	struct BSTNode* parent, *left, *right; /* pointers to parent, left & right subtrees */
};

struct WBSTree
{
	struct BSTNode* tree;  /* root node */
	int count;						 /* number of nodes in tree */

	int (*CMP)(const void* x, const void* y); /* key comparison fp */
	void* (*CTOR)(void* x);  /* Key ADT user-defined c'tor & d'tor */
	void (*DTOR)(void* x);
};

struct WBSTree* WCreateBST(int (*CMP)(const void* x, const void* y),
													void* (*CTOR)(void* x),
													void (*DTOR)(void* x));
void WDeleteBST(struct WBSTree* bst);
void* WMinimumBST(struct WBSTree* bst);
void* WMaximumBST(struct WBSTree* bst);
void* WSearchKeyBST(struct WBSTree* bst, void* key);
int WInsertKeyBST(struct WBSTree* bst, void* key);
int WDeleteKeyBST(struct WBSTree* bst, void* key);

/* ---Sorting Algorithms on Array--- */

void WMergeSortInt(int v[], int i, int j);
void WMergeStepInt(int v[], int i, int m, int j);

void WMergeSort(void* v[], int i, int j, int (*CMP)(const void* x, const void* y));
void WMergeStep(void* v[], int i, int m, int j, int (*CMP)(const void* x, const void* y));

void WMaxHeapifyInt(int v[], int Len, int indx);
void WBuildMaxHeapInt(int v[], int Len, int* heapSize);
void WHeapSortInt(int v[], int Len);

void WMaxHeapify(void* v[], int Len, int indx, int (*CMP)(const void* x, const void* y));
void WBuildMaxHeap(void* v[], int Len, int* heapSize, int (*CMP)(const void* x, const void* y));
void WHeapSort(void* v[], int Len, int (*CMP)(const void* x, const void* y));

#endif /* C_ALGORITHM_H */
#ifndef C_ALGORITHM_H
#define C_ALGORITHM_H

/*
	 C algorithms on Lists, Sets & Graphs.
	 All APIs, ADTs & Algos are all pointer based with generic types ie. 'void*'
	 using heap allocated memory only! Will crash on memory outage.
*/

#define ABS(x)	((x) < 0)?(-(x)):(x)
#define MIN(a, b)	((a) < (b) ? (a) : (b))
#define MAX(a, b)	((a) > (b) ? (a) : (b))

#define FALSE	0
#define TRUE	1
#define NUL	((char)'\0')

typedef int (*WCMPFP)(const void* x, const void* y);
typedef void* (*WCTRFP)(void* x);
typedef void (*WDTRFP)(void* x);

/* Error conditions returned by some of the APIs */
enum {
	WESUCCESS = 0,
	WENOMEMORY = -1,
	WEKEYNOTFND = -2,
	WEINCKEYLESS = -3,
	WEUNIQUEKEY = -4, /* Only unique keys allowed, key already exists */
	WESELFLOOP = -5, /* Self loop not allowed in undirected graph */
	WEEDGALRDYEXTS = -6, /* Edge already exists in undirected Graph */
	WEEDGNOTFND = -7, /* Edge not found in undirected Graph */
	WEPATHNOTFND = -8, /* Path between s and v not found, disconnected tree */
	WEDECKEYGRTR = -9, /* New key is greater than current key */
	WEANYDSTEMPTY = -10 /* DST empty before delete is called on a key */
};

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
void WIteratorList2(struct WLList* l, void (*ITR)(void*, void*));
void WIteratorList3(struct WLList* l, void* arg, void (*ITR)(void*, void*));
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
void WIteratorBST(struct WBSTree* bst, void (*ITR)(void*));

/* ---Red-Black Tree (balanced Binary Search Tree) ADT--- */

enum { /* Color of nodes in RBTree */
	WRBCLR_RED = 1,
	WRBCLR_BLACK
};

struct RBTNode
{
	void* data; /* key ADT */
	int color;  /* Color of node */
	struct RBTNode* parent, * left, * right; /* pointers to parent, left & right subtrees */
};

struct WRBTree
{
	struct RBTNode* tree;  /* root node */
	struct RBTNode* nil;   /* NIL node, all leaves & roots parent */
	int count;						 /* number of nodes in tree */

	int (*CMP)(const void* x, const void* y); /* key comparison fp */
	void* (*CTOR)(void* x);  /* Key ADT user-defined c'tor & d'tor */
	void (*DTOR)(void* x);
};

struct WRBTree* WCreateRBT(int (*CMP)(const void* x, const void* y),
													void* (*CTOR)(void* x),
													void (*DTOR)(void* x));
void WDeleteRBT(struct WRBTree* rbt);
void* WMinimumRBT(struct WRBTree* rbt);
void* WMaximumRBT(struct WRBTree* rbt);
void* WSearchKeyRBT(struct WRBTree* rbt, void* key);
int WInsertKeyRBT(struct WRBTree* rbt, void* key);
int WDeleteKeyRBT(struct WRBTree* rbt, void* key);
void WIteratorRBT(struct WRBTree* rbt, void (*ITR)(void*));

/* ---Sets based on Red-Black Tree ADT--- */

struct WSet {
	struct WRBTree* rbt; /* set is unique key unordered, other ADTs are multisets */
};

struct WSet* WCreateSet(int (*CMP)(const void* x, const void* y),
												void* (*CTOR)(void* x),
												void (*DTOR)(void* x));
void WDeleteSet(struct WSet* set);
int WInsertKeySet(struct WSet* set, void* key);
int WDeleteKeySet(struct WSet* set, void* key);

/* Both sets s, t must be homogeneous, with same types */
int WUnionOfSet(struct WSet* s, struct WSet* t, struct WSet** u);
int WIntersectionOfSet(struct WSet* s, struct WSet* t, struct WSet** i);
int WMinusOfSet(struct WSet* s, struct WSet* t, struct WSet** m);
int WIsNullSet(struct WSet* set);
void WIteratorSet(struct WSet* set, void (*ITR)(void*));

/* ---Graphs (undirected) based on Adjacency Lists ADT--- */

enum {
	WGRPHCLR_WHITE = 1,
	WGRPHCLR_GRAY,
	WGRPHCLR_BLACK
};

struct Vertex /* Vertices V in G, set of actual vertices */
{
	void* data;
	int color;
	int d; /* Distance */
	struct Vertex* p;  /* parent vertex in BFS tree */
	struct AdjacencyList* Adj; /* edges adjacent to vertex u */
	struct Vertex* next;
};

struct AdjacencyList /* Edge E in G, set of edges of u */
{
	struct Vertex* v; /* Reference to vertex adjacent to u */
	struct AdjacencyList* next;
};

struct WGraph /* G = (V, E) */
{
	struct Vertex* V;  /* List of dynamically added vertices */
	int count;  /* Number of vertices */
	int (*CMP)(const void* x, const void* y); /* key comparison fp */
	void* (*CTOR)(void* x);  /* Key ADT user-defined c'tor & d'tor */
	void (*DTOR)(void* x);
};

struct WGraph* WCreateGraph(int (*CMP)(const void* x, const void* y),
														void* (*CTOR)(void* x),
														void (*DTOR)(void* x));
void WDeleteGraph(struct WGraph* G);
int WInsertVertexToGraph(struct WGraph* G, void* key);
int WDeleteVertexFrmGraph(struct WGraph* G, void* key);
int WAddEdgeToGraph(struct WGraph* G, void* uKey, void* vKey);
int WDeleteEdgeFrmGraph(struct WGraph* G, void* uKey, void* vKey);
int WBreadthFirstSearchGraph(struct WGraph* G, void* key, void (*VISITUFP)(void*,int));
void WTraceBFSTreeOnGraph(struct WGraph* G, void* sKey, void* vKey, void (*VISITUFP)(void*,int));
int WDepthFirstSearchGraph(struct WGraph* G, void (*VISITUFP)(void*, int));

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

void WQuickSort(void* v[], int left, int right, int (*cmp)(const void*, const void*));

/* ---Conversion of one ADT to another ADT--- */

int WConvertSetToGraph(struct WSet* S, struct WGraph** G);
int WConvertSetToGraph2(struct WSet* S, struct WGraph** G, WCTRFP ctr, WDTRFP dtr);

/* ---Fibonacci (Min) Heap O(1), built with circular-doubly linked list ADT--- */

struct FibNode
{
	void* data; /* key of Heap nodes */
	struct FibNode* p; /* parent of node */
	struct FibNode* child; /* child list pointer */
	struct FibNode* left; /* left sibling of child in circular-doubly linked list  */
	struct FibNode* right; /* right sibling of child in circular-doubly linked list */
	int degree; /* the number of children in child list */
	int mark; /* True or False */
};

struct WFibHeap
{
	struct FibNode* rootList; /* list of rooted-trees that are min-heap ordered c-d-ll */
	struct FibNode* min;  /* points to node in rootlist whose key is minimum */
	int count; /* n the number of nodes currently in H */
	int (*CMP)(const void* x, const void* y); /* key comparison fp */
	void* (*CTOR)(void* x);  /* Key ADT user-defined c'tor & d'tor */
	void (*DTOR)(void* x);
};

struct WFibHeap* WCreateFibHeap(int (*CMP)(const void* x, const void* y),
																void* (*CTOR)(void* x),
																void (*DTOR)(void* x));
void WDeleteFibHeap(struct WFibHeap* fh);
int WInsertKeyFibHeap(struct WFibHeap* fh, void* key);
struct WFibHeap* WUnionFibHeap(struct WFibHeap* fh1, struct WFibHeap* fh2);
void* WExtractMinFrmFibHeap(struct WFibHeap* fh);
int WDecreaseKeyFibHeap(struct WFibHeap* fh, void* key, void* newKey);
int WDecreaseKeyFibHeap2(struct WFibHeap* fh, void* key,WCMPFP cmp,\
												 void (*updte)(void*, void*), void* updtArg);
int WDeleteKeyFibHeap(struct WFibHeap* fh, void* key);

#endif /* C_ALGORITHM_H */
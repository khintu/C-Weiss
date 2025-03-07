#ifndef RANDOMIZED_H
#define RANDOMIZED_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <c-algorithm.h>

/* Random number generator */
uint32_t WRandom(void);

/* Skip List, ordered fast search datastructure */
struct SKLArry {
	struct SKLArry* pNext; // Pointer to next node in the i'th list
	struct SKLNode* pBase; // Pointer to user data of a node
};

struct SKLNode {
	struct SKLArry* pSet; // Pointer to Array of skip-level, i'th layer list node
	void* pData; // Pointer to user data
};

struct WSkipList {
	double Pr; // Probability of a node to be in the next level
	int32_t approxLen; // Approximate length of the list
	int32_t maxLevel; // Maximum level of the list
	int32_t len; // Current length of the list
	WCMPFP CMP; // Pointer to user defined comparison function
	WCTRFP CTR; // Pointer to user defined constructor function
	WDTRFP DTR; // Pointer to user defined destructor function
	struct SKLNode* pHead; // Pointer to head node of the list
};

struct WSkipList* WCreateSkipList(double, int32_t, WCTRFP, WDTRFP, WCMPFP);
int32_t WInsertSkipList(struct WSkipList*, void*);
void* WSearchSkipList(struct WSkipList*, void*);
int32_t WDeleteFrmSkipList(struct WSkipList*, void*);
void WDeleteSkipList(struct WSkipList*);
void WIterateSkipList(struct WSkipList*, void (*)(void*));

/* Randomized Binary Search Tree, Treaps, ordered fast search ds */
struct TreapNode {
	struct TreapNode* left; // Pointer to left child
	struct TreapNode* right; // Pointer to right child
	void* data; // Pointer to user data
	uint32_t priority; // Priority of the node
};

struct WTreap {
	WCMPFP CMP; // Pointer to user defined comparison function
	WCTRFP CTR; // Pointer to user defined constructor function
	WDTRFP DTR; // Pointer to user defined destructor function
	struct TreapNode* root; // Pointer to root node of the tree
	uint32_t count; // Number of nodes in the tree
};

struct WTreap* WCreateTreap(WCTRFP, WDTRFP, WCMPFP);
int32_t WInsertInTreap(struct WTreap*, void*);
void* WSearchInTreap(struct WTreap*, void*);
int32_t WDeleteFrmTreap(struct WTreap*, void*);
void WDeleteTreap(struct WTreap*);
void WIterateTreap(struct WTreap*, void (*)(void*, uint32_t));

#endif /* RANDOMIZED_H */
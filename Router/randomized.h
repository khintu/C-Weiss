#ifndef RANDOMIZED_H
#define RANDOMIZED_H

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <c-algorithm.h>


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
int WInsertSkipList(struct WSkipList*, void*);
void* WSearchSkipList(struct WSkipList*, void*);
int WDeleteFrmSkipList(struct WSkipList*, void*);
void WDeleteSkipList(struct WSkipList*);
void WIterateSkipList(struct WSkipList*, void (*)(void*));

#endif /* RANDOMIZED_H */
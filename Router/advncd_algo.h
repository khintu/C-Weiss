#ifndef ADVNCD_ALGO_H
#define ADVNCD_ALGO_H

#include <stdint.h>
#include <stdlib.h>
#include <c-algorithm.h>

/* Misc Algorithms */
void* WAppendToList2(struct WLList*, void*);
void WIteratorList2(struct WLList*, void (*)(void*, void*));
void WIteratorList2Arg(struct WLList*, void*, void (*)(void*, void*, void*));
void WIteratorList3(struct WLList*, void*, void (*)(void*, void*));
void WIteratorList4(struct WLList*, void*, void*, void (*)(void*, void*, void*));
void* WGetNthData(struct WLList*, uint32_t);

/* Binomial Heap */
struct BnmTree {
	void* data;								/* user data/key */
	uint32_t korder;					/* k-Order 2^i tree size, Bi use as component of Forest Fn */
	struct BnmTree* chldNxt;	/* Decomposition child trees of this Bi starting from 2^0 to 2^(i-1) */
	struct BnmTree* sblgNxt;	/* Sibling chain at the same level of Bi */
};

struct BnmForest {
	uint32_t nOfFn;					/* Number n of data/keys, use for decimal to binary decomposition, Sum 2^i, i>=0 */
	struct BnmTree* trNxt;	/* list of Bi trees from each bit in binary decomposition starting from LSB */
	struct BnmTree* top;		/* min or max value of the Forest Fn */
};

struct WBnmHeap {
	WCMPFP CMP;				/* Pointer to user defined comparison function */
	WCTRFP CTR;				/* Pointer to user defined constructor function */
	WDTRFP DTR;				/* Pointer to user defined destructor function */
	struct BnmForest* Fn;
};

struct WBnmHeap* WCreateBnmHeap(WCTRFP CTR, WDTRFP DTR, WCMPFP CMP);
void WDeleteBnmHeap(struct WBnmHeap* hp);
void WInsertKeyBnmHeap(struct WBnmHeap* hp, void* key);
void* WFindRootBnmHeap(struct WBnmHeap* hp);
void* WExtractRootBnmHeap(struct WBnmHeap* hp);
int32_t WDeleteKeyBnmHeap(struct WBnmHeap* hp, void *key);
int32_t WUpdateKeyBnmHeap(struct WBnmHeap* hp, void* key, void* newKey);
struct WBnmHeap* WUnionBnmHeap(struct WBnmHeap* hp1, struct WBnmHeap* hp2);

#endif /* ADVNCD_ALGO_H */
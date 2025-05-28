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

/* ---Hash Map2(Open Addressing with linear probing) Dictionary ADT--- */

struct KeyNode
{
	void* key;
	void* value;
};

// This ADT allows duplicate keys so user must clean up all keys (multimap)
struct WHashMap2
{
	struct KeyNode** table; /* dynamically allocated array of chains */
	uint32_t tabSize;						/* Size of universal key set */
	int (*KEYCMP)(const void*, const void*);
	void* (*CTORK)(void* x); /* Constructor/Destructor for key/value */
	void (*DTORK)(void* x);
	void* (*CTORV)(void* x);
	void (*DTORV)(void* x);
	uint32_t (*HASHFN)(const void*); /* Hash value from a hash function */
};

struct WHashMap2* WCreateHashMap2(uint32_t tabSize, uint32_t (*HASHFN)(const void*), \
																int32_t (*KCMP)(const void*, const void*), \
																void* (*CTORK)(void*), \
																void (*DTORK)(void*), \
																void* (*CTORV)(void*), \
																void (*DTORV)(void*));
void WDeleteHashMap2(struct WHashMap2*);
void* WSearchKeyHashMap2(struct WHashMap2*, void* key);
int32_t WInsertKeyValHashMap2(struct WHashMap2* hmap, void* key, void* val);
int32_t WDeleteKeyHashMap2(struct WHashMap2* hmap, void* key);
uint32_t WGetSizeHashMap2(struct WHashMap2* hmap);

#endif /* ADVNCD_ALGO_H */
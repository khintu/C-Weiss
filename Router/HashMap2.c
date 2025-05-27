#include <advncd_algo.h>

struct WHashMap2* WCreateHashMap2(uint32_t tabSize, uint32_t (*HASHFN)(const void*), \
																	int32_t (*KCMP)(const void*, const void*), \
																	void* (*CTORK)(void*), \
																	void (*DTORK)(void*), \
																	void* (*CTORV)(void*), \
																	void (*DTORV)(void*))
{
	struct WHashMap2* hmap = (struct WHashMap2*)calloc(1, sizeof *hmap);
	if (!hmap) 
		return NULL;
	hmap->tabSize = tabSize;
	hmap->HASHFN = HASHFN;
	hmap->KEYCMP = KCMP;
	hmap->CTORK = CTORK;
	hmap->DTORK = DTORK;
	hmap->CTORV = CTORV;
	hmap->DTORV = DTORV;
	hmap->table = (struct KeyNode**)calloc(tabSize, sizeof **hmap->table);
	if (!hmap->table) {
		free(hmap);
		return NULL;
	}
	return hmap;
}

void WDeleteHashMap2(struct WHashMap2* hmap)
{
	uint32_t i;
	for (i = 0; i < hmap->tabSize; i++) {
		if (hmap->table[i]) {
			hmap->DTORK(hmap->table[i]->key);
			hmap->DTORV(hmap->table[i]->value);
		}
	}
	free(hmap->table);
	free(hmap);
	return;
}

void* WSearchKeyHashMap2(struct WHashMap2* hmap, void* key)
{
	uint32_t i, idx;
	idx = hmap->HASHFN(key) % hmap->tabSize;
	if (!hmap->table[idx]) 
		return NULL;
	if (hmap->KEYCMP(hmap->table[idx]->key, key) == 0) 
		return hmap->table[idx]->value;
	// Collision resolution by linear probing
	for (i = idx + 1; i >= 0 && i != idx ; ++i) {
		if (i >= hmap->tabSize) {
			i = 0; // wrap around
			continue;
		}
		if (hmap->KEYCMP(hmap->table[i]->key, key) == 0)
			return hmap->table[i]->value;
	}
	return NULL;
}

int32_t WInsertKeyValHashMap2(struct WHashMap2* hmap, void* key, void* val)
{
	uint32_t i, idx;
	
	idx = hmap->HASHFN(key) % hmap->tabSize;
	if (hmap->table[idx]) {
		// Collision resolution by linear probing
		for (i = idx + 1; i >= 0 && i != idx; ++i) {
			if (i >= hmap->tabSize) {
				i = 0; // wrap around
				continue;
			}
			if (!hmap->table[i])
				break;
		}
		if (i == idx)
			return WENOMEMORY;
		idx = i;
	}	
	if (!(hmap->table[idx] = (struct KeyNode*)calloc(1, sizeof *hmap->table[idx])))
		return WENOMEMORY;
	hmap->table[idx]->key = hmap->CTORK(key);
	hmap->table[idx]->value = hmap->CTORV(val);
	return WESUCCESS;
}

int32_t WDeleteKeyHashMap2(struct WHashMap2* hmap, void* key)
{
	uint32_t i, idx;

	idx = hmap->HASHFN(key) % hmap->tabSize;
	if (!hmap->table[idx])
		return WEKEYNOTFND;
	if (hmap->KEYCMP(hmap->table[idx]->key, key) != 0) {
		// Collision resolution by linear probing
		for (i = idx + 1; i >= 0 && i != idx; ++i) {
			if (i >= hmap->tabSize) {
				i = 0; // wrap around
				continue;
			}
			if (hmap->KEYCMP(hmap->table[idx]->key, key) == 0)
				break;
		}
		if (i == idx)
			return WEKEYNOTFND;
		idx = i;
	}
	hmap->DTORK(hmap->table[idx]->key);
	hmap->DTORV(hmap->table[idx]->value);
	free(hmap->table[idx]);
	hmap->table[idx] = NULL;
	return WESUCCESS;
}
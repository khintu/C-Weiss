#include <stdlib.h>
#include <c-algorithm.h>

#define WHASHFUNC(key)	((*hmap->HASHFN)(key) % (hmap->tabSize))

struct WHashMap* WCreateHashMap(int tabSize, int (*HASHFN)(const void*),\
																int (*KCMP)(const void*, const void*),\
																void* (*CTORK)(void* ),\
																void (*DTORK)(void* ),\
																void* (*CTORV)(void* ),\
																void (*DTORV)(void* ))
{
	struct WHashMap* hmap;
	if ((hmap = (struct WHashMap*)calloc(1, sizeof(struct WHashMap))) == NULL)
		return NULL;
	if ((hmap->table = (struct ChainItem**)calloc(tabSize, sizeof(struct ChainItem*))) == NULL)
	{
		free(hmap);
		return NULL;
	}
	hmap->tabSize = tabSize;
	hmap->KEYCMP = KCMP;
	hmap->HASHFN = HASHFN;
	hmap->CTORK = CTORK;
	hmap->CTORV = CTORV;
	hmap->DTORK = DTORK;
	hmap->DTORV = DTORV;
	return hmap;
}

void WDeleteHashMap(struct WHashMap* hmap)
{
	int i;
	struct ChainItem* ch, *prev;
	for (i = 0; i < hmap->tabSize; ++i)
	{
		if ((ch = hmap->table[i]) != NULL)
		{
			while (ch != NULL)
			{
				prev = ch, ch = ch->next;
				hmap->DTORK(prev->key);
				hmap->DTORV(prev->value);
				free(prev);
			}
		}
	}
	free(hmap->table);
	free(hmap);
	return;
}

void* WSearchKeyHashMap(struct WHashMap* hmap, void* key)
{
	struct ChainItem* ch;
	if ((ch = hmap->table[WHASHFUNC(key)]) != NULL)
	{
		while (ch != NULL)
		{
			if (hmap->KEYCMP(ch->key, key) == 0)
				return ch->value;
			ch = ch->next;
		}
	}
	return NULL;
}

int WInsertKeyValHashMap(struct WHashMap* hmap, void* key, void* val)
{
	struct ChainItem* ch, *prev, *nch;
	
	if ((nch = (struct ChainItem*)calloc(1, sizeof(struct ChainItem))) == NULL)
		return -1;

	if ((ch = hmap->table[WHASHFUNC(key)]) == NULL) /* Add new chain and key */
	{
		hmap->table[WHASHFUNC(key)] = ch = nch;
		ch->key = hmap->CTORK(key);
		ch->value = hmap->CTORV(val);
	}
	else
	{
		for (prev = ch; ch != NULL; prev = ch, ch = ch->next)
		{
			if (hmap->KEYCMP(key, ch->key) == 0) /* Update Existing key */
			{
				hmap->DTORV(ch->value);
				ch->value = hmap->CTORV(val);
				free(nch); /* new node not needed, free */
				return 0;
			}
		}
		/* Add new value for key */
		prev->next = ch = nch;
		ch->key = hmap->CTORK(key);
		ch->value = hmap->CTORV(val);
	}
	return 0;
}

int WDeleteKeyHashMap(struct WHashMap* hmap, void* key)
{
	struct ChainItem* ch, *prev;

	if ((ch = hmap->table[WHASHFUNC(key)]) != NULL)
	{
		for (prev = ch; \
				 ch != NULL && hmap->KEYCMP(key, ch->key) != 0;\
				 prev = ch, ch = ch->next)
			;
		if (ch != NULL) /* Found the key, caliberate the chain */
		{
			hmap->DTORK(ch->key);
			hmap->DTORV(ch->value);
			if (hmap->table[WHASHFUNC(key)] == ch) /* Chain head deletion case */
				hmap->table[WHASHFUNC(key)] = ch->next;
			else
				prev->next = ch->next;
			free(ch);
			return 0;
		}
	}
	return -2; /* key not found */
}
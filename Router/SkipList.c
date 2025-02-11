#include <randomized.h>

struct WSkipList* WCreateSkipList(double Pr, int32_t approxLen, WCTRFP CTR, WDTRFP DTR, WCMPFP CMP)
{
	struct WSkipList* sklist;

	sklist = (struct WSkipList*)malloc(sizeof * sklist);
	sklist->maxLevel = (int32_t)(log(approxLen) / log(1 / Pr));
	sklist->pHead	= (struct SKLNode*)calloc(1, sizeof * sklist->pHead);
	sklist->pHead->pSet = (struct SKLArry*)calloc(sklist->maxLevel, sizeof * sklist->pHead->pSet);
	sklist->Pr = Pr;
	sklist->approxLen = approxLen;
	sklist->len = 0;
	sklist->CMP = CMP;
	sklist->CTR = CTR;
	sklist->DTR = DTR;
	return sklist;
}

int WInsertSkipList(struct WSkipList* sklist, void* usrData)
{
	int i, level;
	struct SKLNode* pNewNode, *pItr, *pPrev;

	pNewNode = (struct SKLNode*)calloc(1, sizeof * pNewNode);
	pNewNode->pSet = (struct SKLArry*)calloc(sklist->maxLevel, sizeof * pNewNode->pSet);
	level = (int32_t)(log(sklist->len+1) / log(1 / sklist->Pr)); // Randomize if needed
	
	for (i = sklist->maxLevel - 1, pPrev = sklist->pHead; i >= 0; i--) {
		pItr = (pPrev->pSet[i].pNext)?(pPrev->pSet[i].pNext)->pBase:NULL;
		while (pItr != NULL && sklist->CMP(usrData, pItr->pData) >= 0) {
			pPrev = pItr;
			pItr = (pItr->pSet[i].pNext) ? (pItr->pSet[i].pNext)->pBase:NULL;
		}
		if (i <= level) {
			pPrev->pSet[i].pNext = &pNewNode->pSet[i];
			pNewNode->pSet[i].pNext = (pItr && pItr->pSet[i].pNext)?&pItr->pSet[i]:NULL;
			pNewNode->pSet[i].pBase = pNewNode;
		}
	}

	pNewNode->pData = sklist->CTR(usrData);
	sklist->len++;
	return WESUCCESS;
}

void* WSearchSkipList(struct WSkipList* sklist, void* key)
{
	int i;
	struct SKLNode* pItr, *pPrev;

	for (i = sklist->maxLevel - 1, pPrev = sklist->pHead; i >= 0; i--) {
		pItr = (pPrev->pSet[i].pNext) ? (pPrev->pSet[i].pNext)->pBase : NULL;
		while (pItr != NULL && sklist->CMP(key, pItr->pData) != 0) {
			pPrev = pItr;
			pItr = (pItr->pSet[i].pNext) ? (pItr->pSet[i].pNext)->pBase : NULL;
		}
		if (pItr != NULL)
			return pItr->pData;
	}
	return NULL;
}

int WDeleteFrmSkipList(struct WSkipList* sklist, void* key)
{
	int i;
	struct SKLNode* pItr = NULL, * pPrev;

	for (i = sklist->maxLevel - 1, pPrev = sklist->pHead; i >= 0; i--) {
		pItr = (pPrev->pSet[i].pNext) ? (pPrev->pSet[i].pNext)->pBase : NULL;
		while (pItr != NULL && sklist->CMP(key, pItr->pData) != 0) {
			pPrev = pItr;
			pItr = (pItr->pSet[i].pNext) ? (pItr->pSet[i].pNext)->pBase : NULL;
		}
		if (pItr != NULL)
			pPrev->pSet[i].pNext = pItr->pSet[i].pNext;
	}
	if (pItr) {
		free(pItr->pSet);
		sklist->DTR(pItr->pData);
		free(pItr);
		sklist->len--;
		return WESUCCESS;
	}
	return WEKEYNOTFND;
}

void WDeleteSkipList(struct WSkipList* sklist)
{
	struct SKLNode* pItr, *pNext;
	for (pItr = (sklist->pHead->pSet[0].pNext)?(sklist->pHead->pSet[0].pNext)->pBase:NULL;\
			 pItr != NULL; pItr = pNext){
		pNext = (pItr->pSet[0].pNext)? (pItr->pSet[0].pNext)->pBase:NULL;
		free(pItr->pSet);
		sklist->DTR(pItr->pData);
		free(pItr);
		sklist->len--;
	}
	free(sklist->pHead->pSet);
	free(sklist->pHead);
	free(sklist);
	return;
}
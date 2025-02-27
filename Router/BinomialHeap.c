#include <advncd_algo.h>
#include <math.h>

/* Make x child of y, increment y order */
#define COUPLING(x, y) {\
												if (chld = (y)->chldNxt) {\
													while (chld->sblgNxt != NULL && (chld = chld->sblgNxt));\
													chld->sblgNxt = (x);\
												}\
												else\
													(y)->chldNxt = (x);\
												(y)->korder++;\
												(x)->sblgNxt = (y)->sblgNxt = NULL;\
											}

/* Both Bi's must be of the same order in their respective Fn (MinHeap) */
static struct BnmTree* couplingBnmTreeComponent(struct WBnmHeap* hp,
																								struct BnmTree* b1,
																								struct BnmTree* b2,
																								struct BnmTree** carry)
{
	struct BnmTree* chld, *retval;

	if (b1 == NULL && b2 == NULL && *carry == NULL)
		return NULL;
	else if (*carry != NULL) {
		if (b2 && !b1) {
			if (hp->CMP((*carry)->data, b2->data) > 0) {
				COUPLING(*carry, b2);
				*carry = b2;
			}
			else {
				COUPLING(b2, *carry);
			}
			return NULL;
		}
		else if (!b2 && b1) {
			if (hp->CMP((*carry)->data, b1->data) > 0) {
				COUPLING(*carry, b1);
				*carry = b1;
			}
			else {
				COUPLING(b1, *carry);
			}
			return NULL;
		}
		else if (b1 && b2) {
			if (hp->CMP((*carry)->data, b1->data) > 0) {
				COUPLING(*carry, b1);
				*carry = b1;
			}
			else {
				COUPLING(b1, *carry);
			}
			b2->sblgNxt = NULL;
			return b2;
		}
		else {
			retval = *carry;
			*carry = NULL;
			return retval;
		}
	}
	else {
		if (b1 && b2) {
			if (hp->CMP(b1->data, b2->data) > 0) {
				COUPLING(b1, b2);
				*carry = b2;
			}
			else {
				COUPLING(b2, b1);
				*carry = b1;
			}
			return NULL;
		}
		else if (b1) {
			b1->sblgNxt = NULL;
			return b1;
		}
		else {
			b2->sblgNxt = NULL;
			return b2;
		}
	}
}

static void attachToForest(struct WBnmHeap* hp, struct BnmForest* F, struct BnmTree* B)
{
	struct BnmTree* Bx;
	for (Bx = F->trNxt; Bx && Bx->sblgNxt != NULL; Bx = Bx->sblgNxt)
		;
	if (Bx == NULL)
		F->trNxt = B;
	else
		Bx->sblgNxt = B;
	
	/* Top (Min/Max) of Forest assignment */
	if (F->top == NULL)
		F->top = B;
	else {
		if (hp->CMP(F->top->data, B->data) > 0) /* MinHeap */
			F->top = B;
	}
	return;
}

/* Union of Fi to Fj, using coupling of binary components, return new F, delete Fi&Fj */
static struct BnmForest* UnionFiToFj(struct WBnmHeap* hp, struct BnmForest* Fi, struct BnmForest* Fj)
{
	struct BnmTree* Bx, * Bxi, * Bxj, * Bcarry, *BxiNxt, *BxjNxt;
	struct BnmForest* Fnew;
	uint32_t maxOrder, i;

	maxOrder = 0;
	for (Bx = Fi->trNxt; Bx != NULL; Bx = Bx->sblgNxt)
		maxOrder = MAX(maxOrder, Bx->korder);
	for (Bx = Fj->trNxt; Bx != NULL; Bx = Bx->sblgNxt)
		maxOrder = MAX(maxOrder, Bx->korder);

	Fnew = (struct BnmForest*)calloc(1, sizeof * Fnew);

	for (Bcarry = NULL, Bxi = Fi->trNxt, Bxj = Fj->trNxt, i = 0; i <= maxOrder; ++i) {
		if (Bxi && Bxj) {
			if (Bxi->korder == i && Bxj->korder == i) {
				BxiNxt = Bxi->sblgNxt, BxjNxt = Bxj->sblgNxt;
				Bx = couplingBnmTreeComponent(hp, Bxi, Bxj, &Bcarry);
				if (Bx != NULL)
					attachToForest(hp, Fnew, Bx);
				Bxi = BxiNxt, Bxj = BxjNxt;
			}
			else if (Bxi->korder == i && Bxj->korder != i) {
				BxiNxt = Bxi->sblgNxt;
				Bx = couplingBnmTreeComponent(hp, Bxi, NULL, &Bcarry);
				if (Bx != NULL)
					attachToForest(hp, Fnew, Bx);
				Bxi = BxiNxt;
			}
			else if (Bxj->korder == i && Bxi->korder != i) {
				BxjNxt = Bxj->sblgNxt;
				Bx = couplingBnmTreeComponent(hp, NULL, Bxj, &Bcarry);
				if (Bx != NULL)
					attachToForest(hp, Fnew, Bx);
				Bxj = BxjNxt;
			}
			else {
				if (Bcarry) /* Intermediate carry but both Bxi&Bxj not of i order */
					attachToForest(hp, Fnew, Bcarry);
				Bcarry = NULL;
			}
		}
		else if (Bxi) {
			if (Bxi->korder == i) {
				BxiNxt = Bxi->sblgNxt;
				Bx = couplingBnmTreeComponent(hp, Bxi, NULL, &Bcarry);
				if (Bx != NULL)
					attachToForest(hp, Fnew, Bx);
				Bxi = BxiNxt;
			}
			else {
				if (Bcarry) /* Intermediate carry, if any */
					attachToForest(hp, Fnew, Bcarry);
				Bcarry = NULL;
			}
		}
		else {
			if (Bxj->korder == i) {
				BxjNxt = Bxj->sblgNxt;
				Bx = couplingBnmTreeComponent(hp, NULL, Bxj, &Bcarry);
				if (Bx != NULL)
					attachToForest(hp, Fnew, Bx);
				Bxj = BxjNxt;
			}
			else {
				if (Bcarry) /* Intermediate carry, if any */
					attachToForest(hp, Fnew, Bcarry);
				Bcarry = NULL;
			}
		}
	}
	if (Bcarry) /* Last Carry */
		attachToForest(hp, Fnew, Bcarry);
	
	/* Update n of Fnew */
	Fnew->nOfFn = Fi->nOfFn + Fj->nOfFn;
	
	free(Fi); free(Fj);
	return Fnew;
}


struct WBnmHeap* WCreateBnmHeap(WCTRFP CTR, WDTRFP DTR, WCMPFP CMP)
{
	struct WBnmHeap* hp;

	hp = (struct WBnmHeap*)calloc(1, sizeof * hp);
	hp->CMP = CMP;
	hp->CTR = CTR;
	hp->DTR = DTR;
	return hp;
}

/* Deleting a tree Bk which is an order of k, with 2^k nodes */
static struct BnmTree* deleteBiComponent(struct WBnmHeap* hp, struct BnmTree* Bi)
{
	struct BnmTree* parent, * chld;

	parent = Bi, chld = Bi->chldNxt;
	while (chld != NULL) {
		chld = deleteBiComponent(hp, chld);
	}
	Bi = Bi->sblgNxt;
	hp->DTR(parent->data);
	free(parent);
	hp->Fn->nOfFn--;
	return Bi;
}

void WDeleteBnmHeap(struct WBnmHeap* hp)
{
	struct BnmTree* Bi;
	
	if (hp) {
		Bi = hp->Fn->trNxt; /* Start from LSB to MSB in list */
		while (Bi != NULL) {
			Bi = deleteBiComponent(hp, Bi);
		}
		free(hp->Fn);
		free(hp);
	}
	return;
}

void WInsertKeyBnmHeap(struct WBnmHeap* hp, void* key)
{
	struct BnmTree* Bx;
	struct BnmForest* Fx;

	if (hp->Fn == NULL) {
		hp->Fn = (struct BnmForest*)calloc(1, sizeof * (hp->Fn));
		Bx = (struct BnmTree*)calloc(1, sizeof * Bx);
		Bx->data = hp->CTR(key);
		hp->Fn->trNxt = Bx;
		hp->Fn->top = Bx;
		hp->Fn->nOfFn++;
	}
	else {
		Fx = (struct BnmForest*)calloc(1, sizeof * Fx);
		Bx = (struct BnmTree*)calloc(1, sizeof * Bx);
		Bx->data = hp->CTR(key);
		Fx->trNxt = Bx;
		Fx->top = Bx;
		Fx->nOfFn++;

		hp->Fn = UnionFiToFj(hp, Fx, hp->Fn);
	}
	return;
}

void* WFindRootBnmHeap(struct WBnmHeap* hp)
{
	if (hp->Fn && hp->Fn->top)
		return hp->Fn->top->data;
	return NULL;
}

static struct BnmForest* FrstCtr(struct BnmForest* x)
{
	return x;
}

static void FrstDtr(struct BnmForest* x)
{
	return;
}

static int32_t FrstCmp(struct BnmForest* x, struct BnmForest* y)
{
	if (x == y)
		return 0;
	else
		return -1;
}

static void decomposeBmToFm(struct BnmTree* Bm, struct WLQueue* frstQ)
{
	struct BnmForest* Fx;
	struct BnmTree* Bx, *BxNxt;

	for (Bx = Bm->chldNxt; Bx != NULL; Bx = BxNxt) {
		Fx = (struct BnmForest*)calloc(1, sizeof * Fx);
		Fx->top = Bx;
		Fx->nOfFn = ((int)pow(2.0, (double)Bx->korder));
		Fx->trNxt = Bx;
		BxNxt = Bx->sblgNxt;
		Bx->sblgNxt = NULL;
		WEnqueueLQueue(frstQ, Fx);
	}
	return;
}

static void assignTop2Forest(struct WBnmHeap* hp, struct BnmForest* Fn)
{
	struct BnmTree* Bx;
	for (Bx = Fn->trNxt; Bx != NULL; Bx = Bx->sblgNxt) {
		if (Fn->top == NULL)
			Fn->top = Bx;
		else {
			if (hp->CMP(Fn->top->data, Bx->data) > 0)
				Fn->top = Bx;
		}
	}
	return;
}

static struct BnmTree* recreateFrstFrmFrstQ(struct WBnmHeap* hp,
																						struct BnmForest* Fn,
																						struct BnmTree* Bm,
																						struct WLQueue* frstQ)
{
	struct BnmForest* Fx;
	struct BnmTree* Bx, *BxPrvSblg;
	
	/* First remove Bm from Forest Fn */
	for (Bx = Fn->trNxt, BxPrvSblg = Bx; Bx != Bm; BxPrvSblg = Bx, Bx = Bx->sblgNxt)
		;
	if (Bx == Fn->trNxt)
		Fn->trNxt = Bx->sblgNxt;
	else
		BxPrvSblg->sblgNxt = Bx->sblgNxt;
	Fn->nOfFn -= ((int)pow(2.0, (double)Bm->korder));
	Fn->top = NULL;

	/* Decompose Bm to Fm-1, ie. exclude Bm from Fm */
	decomposeBmToFm(Bm, frstQ);

	/* Compose Fn from each Fx in frstQ incremental Union with Fn */
	if (WIsEmptyLQueue(frstQ)) {
		assignTop2Forest(hp, Fn);
	}
	else {
		while (Fx = WDequeueLQueue(frstQ))
			Fn = UnionFiToFj(hp, Fx, Fn);
		hp->Fn = Fn;
	}

	/* return Bm to ExtractRoot or UpdateRoot */
	return Bm;
}

void* WExtractRootBnmHeap(struct WBnmHeap* hp)
{
	struct WLQueue* frstQ;
	struct BnmTree* Bm;
	void* retval;
	
	if (hp->Fn == NULL || hp->Fn->nOfFn == 0)
		return NULL;

	frstQ = WCreateLQueue((WCMPFP)FrstCmp, (WCTRFP)FrstCtr, (WDTRFP)FrstDtr);
	Bm = recreateFrstFrmFrstQ(hp, hp->Fn, hp->Fn->top, frstQ);
	retval = hp->CTR(Bm->data);
	hp->DTR(Bm->data);
	free(Bm);
	WDeleteLQueue(frstQ);
	return retval;
}

/* Update key, first find key Bm, then delete & disassociate into forest, inc. union */
static int32_t findKeyInBroot(struct WBnmHeap* hp, void* key, struct BnmTree** Broot, struct BnmTree** Bm)
{
	struct BnmTree* chld;
	int32_t flag = FALSE;

	if (hp->CMP(key, (*Broot)->data) == 0) {
		*Bm = *Broot;
		return TRUE;
	}
	chld = (*Broot)->chldNxt;
	while (chld != NULL && flag == FALSE) {
		flag = findKeyInBroot(hp, key, &chld, Bm);
	}
	if (flag != TRUE)
		*Broot = (*Broot)->sblgNxt;
	
	return flag;
}

static int32_t findKeyRootAndNode(struct WBnmHeap* hp, void* key, struct BnmTree** Broot, struct BnmTree** Bm)
{
	struct BnmTree* Bx;

	Bx = hp->Fn->trNxt; /* Start from LSB to MSB in list */
	while (Bx != NULL) {
		if (TRUE == findKeyInBroot(hp, key, &Bx, Bm)) {
			*Broot = Bx;
			return TRUE;
		}
	}
	
	return FALSE;
}

static struct BnmTree* disassociateBrootCompletely(struct WBnmHeap* hp,
																									 struct BnmTree* Broot,
																									 struct BnmTree* Bm,
																									 struct WLQueue* frstQ)
{
	struct BnmTree *chld;
	struct BnmForest* Fx;

	if (Broot != Bm) {
		Fx = (struct BnmForest*)calloc(1, sizeof * Fx);
		chld = Broot->chldNxt;
		while (chld != NULL) {
			chld = disassociateBrootCompletely(hp, chld, Bm, frstQ);
		}
		chld = Broot->sblgNxt;
		Fx->nOfFn = 1;
		Fx->top = Broot;
		Fx->trNxt = Broot;
		Broot->chldNxt = NULL;
		Broot->sblgNxt = NULL;
		Broot->korder = 0;
		WEnqueueLQueue(frstQ, Fx);
	}
	else {
		decomposeBmToFm(Bm, frstQ);
		chld = Bm->sblgNxt;
		hp->DTR(Bm->data);
		free(Bm);
	}
	return chld;
}

int32_t WUpdateKeyBnmHeap(struct WBnmHeap* hp, void* key, void* newKey)
{
	struct BnmForest *Fx;

	if (WESUCCESS != WDeleteKeyBnmHeap(hp, key))
		return WEKEYNOTFND;

	/* Insert new key */
	Fx = (struct BnmForest*)calloc(1, sizeof * Fx);
	Fx->nOfFn = 1;
	Fx->trNxt = (struct BnmTree*)calloc(1, sizeof * Fx->trNxt);
	Fx->top = Fx->trNxt;
	Fx->trNxt->korder = 1;
	Fx->trNxt->data = hp->CTR(newKey);
	hp->Fn = UnionFiToFj(hp, Fx, hp->Fn);

	return WESUCCESS;
}

int32_t WDeleteKeyBnmHeap(struct WBnmHeap* hp, void* key)
{
	struct WLQueue* frstQ;
	struct BnmTree* Bm, * Broot, * Bx, * Bprev;
	struct BnmForest* Fx;

	/* Find existing keys Broot & Bm */
	if (FALSE == findKeyRootAndNode(hp, key, &Broot, &Bm))
		return WEKEYNOTFND;

	frstQ = WCreateLQueue((WCMPFP)FrstCmp, (WCTRFP)FrstCtr, (WDTRFP)FrstDtr);

	/* Remove Broot from Fn */
	for (Bx = hp->Fn->trNxt, Bprev = Bx; Bx != Broot; Bprev = Bx, Bx = Bx->sblgNxt)
		;
	if (Bprev == Bx)
		hp->Fn->trNxt = Bx->sblgNxt;
	else
		Bprev->sblgNxt = Bx->sblgNxt;
	hp->Fn->nOfFn -= (int)pow(2.0, Broot->korder);

	/* Create sinlge node Bx, Fx(s) out of every node in Broot, delete Bm */
	disassociateBrootCompletely(hp, Broot, Bm, frstQ);

	/* Reassemble hp->Fn out of frstQ Fx(s) */
	if (WIsEmptyLQueue(frstQ)) {
		assignTop2Forest(hp, hp->Fn);
	}
	else {
		while (Fx = WDequeueLQueue(frstQ))
			hp->Fn = UnionFiToFj(hp, Fx, hp->Fn);
	}

	WDeleteLQueue(frstQ);
	return WESUCCESS;
}

struct WBnmHeap* WUnionBnmHeap(struct WBnmHeap* hp1, struct WBnmHeap* hp2)
{
	struct WBnmHeap* hp;
	
	hp = WCreateBnmHeap(hp1->CTR, hp1->DTR, hp1->CMP);
	hp->Fn = UnionFiToFj(hp, hp1->Fn, hp2->Fn);
	free(hp1);
	free(hp2);
	
	return hp;
}
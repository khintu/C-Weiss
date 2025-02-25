#include <advncd_algo.h>

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
		else if (b1)
			return b1;
		else
			return b2;
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

	for (maxOrder = 0, Bx = Fi->trNxt; Bx != NULL; Bx = Bx->sblgNxt)
		maxOrder = MAX(maxOrder, Bx->korder);
	for (maxOrder = 0, Bx = Fj->trNxt; Bx != NULL; Bx = Bx->sblgNxt)
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
	
	Bi = hp->Fn->trNxt; /* Start from LSB to MSB in list */
	while (Bi != NULL) {
		Bi = deleteBiComponent(hp, Bi);
	}
	free(hp);
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
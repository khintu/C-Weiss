#include <router_defs.h>

struct DJSCollection* DJSCreateCollection(void)
{
	struct DJSCollection* collctn;
	if ((collctn = (struct DJSCollection*)calloc(1, sizeof * collctn)) == NULL)
		return NULL;
	return collctn;
}

struct DJSet* DJSMakeSet(struct DJSCollection* collctn, void* object)
{
	struct DJSet* s;
	struct DJSetNode* n;
	struct DJSCollection* p;
	if ((n = (struct DJSetNode*)calloc(1, sizeof * n)) == NULL)
		return NULL;
	n->object = object;
	n->next = NULL;
	if ((s = (struct DJSet*)calloc(1, sizeof * s)) == NULL) {
		free(n);
		return NULL;
	}
	n->set = s;
	s->length = 1;
	s->head = n;
	s->tail = n;
	if (collctn->next == NULL) {
		collctn->S = s;
		if ((collctn->next = (struct DJSCollection*)calloc(1, sizeof * collctn)) == NULL) {
			free(n); free(s);
			return NULL;
		}
	}
	else {
		for (p = collctn; p->next != NULL; p = p->next)
			;
		if ((p->next = (struct DJSCollection*)calloc(1, sizeof * p)) == NULL) {
			free(n); free(s);
			return NULL;
		}
		p = p->next;
		p->S = s;
	}
	return s;
}

/* Returns representative from the set found in */
void* DJSFindSet(void* vertex)
{
	return ((struct MSTVertex*)vertex)->setNode->set->head->object;
}

void DJSUnion(struct DJSCollection** collctn, struct DJSet* x, struct DJSet* y)
{
	struct DJSet* max = x, *min = y;
	struct DJSetNode* n;
	struct DJSCollection* p, *prev;
	if (x->length < y->length) {
		max = y;
		min = x;
	}
	max->tail->next = min->head;
	max->tail = min->tail;

	for (n = min->head; n != NULL; n = n->next)
		n->set = max;

	for (p = *collctn, prev = p; p != NULL; prev = p, p = p->next) {
		if (p->S == min) {
			if (p == *collctn)
				*collctn = p->next;
			else
				prev->next = p->next;
			free(p);
			break;
		}
	}
	free(min);
	return;
}

void DJSetDestroy(struct DJSet* s)
{
	struct DJSetNode* p, * p1;
	p = s->head;
	while (p != NULL){
		p1 = p;
		p = p->next;
		free(p1);
	}
	free(s);
	return;
}

void DJSCollectionDestroy(struct DJSCollection* clc)
{
	struct DJSCollection* p, * p1;
	p = clc;
	while (p != NULL) {
		p1 = p;
		p = p->next;
		DJSetDestroy(p1->S);
		free(p1);
	}
	return;
}
#include <stdlib.h>
#include <c-algorithm.h>

/* When we enter this func we are sure that it is 
	 always more than or equal to 2 elements. */
void WMergeStepInt(int v[], int i, int m, int j)
{
	int* L, * R;
	int nL, nR, l, r, I;

	nL = m - i + 1;
	nR = j - m; /* j - (m+1) + 1 */
	if ((L = (int*)malloc(sizeof(int) * nL)) == NULL)
		return;
	if ((R = (int*)malloc(sizeof(int) * nR)) == NULL)
	{
		free(L);
		return;
	}

	for (l = 0; l < nL; ++l)
		L[l] = v[i + l];
	for (r = 0; r < nR; ++r)
		R[r] = v[m + 1 + r];

	for (l = 0, r = 0, I = i; I <= j; ++I)
	{
		if (l < nL && r < nR)
		{
			if (L[l] <= R[r])
				v[I] = L[l++];
			else
				v[I] = R[r++];
		}
		else
		{
			if (l >= nL)
				v[I] = R[r++];
			else /* if (r >= nR) or both overrun */
				v[I] = L[l++];
		}
	}
	free(L);
	free(R);
	return;
}

/* Cormen book, Merge sort with running time O(nLgn) */
void WMergeSortInt(int v[], int i, int j)
{
	int mid;

	if (i >= j) /* Base case, of one element already sorted */
		return;
	mid = (i + j) / 2; /* Divide & Conquer steps */
	WMergeSortInt(v, i, mid);
	WMergeSortInt(v, mid + 1, j);
	WMergeStepInt(v, i, mid, j);
	return;
}

/* Generic Merge Sort */
void WMergeSort(void* v[], int i, int j, int (*CMP)(const void* x, const void* y))
{
	int mid;

	if (i >= j) /* Base case, of one element already sorted */
		return;
	mid = (i + j) / 2; /* Divide & Conquer steps */
	WMergeSort(v, i, mid, CMP);
	WMergeSort(v, mid + 1, j, CMP);
	WMergeStep(v, i, mid, j, CMP);
	return;
}

void WMergeStep(void* v[], int i, int m, int j, int (*CMP)(const void* x, const void* y))
{
	void** L, **R; /* Array of (void*) */
	int nL, nR, l, r, I;

	nL = m - i + 1;
	nR = j - m; /* j - (m+1) + 1 */
	if ((L = (void**)malloc(sizeof(void*) * nL)) == NULL)
		return;
	if ((R = (void**)malloc(sizeof(void*) * nR)) == NULL)
	{
		free(L);
		return;
	}

	for (l = 0; l < nL; ++l)
		L[l] = v[i + l];
	for (r = 0; r < nR; ++r)
		R[r] = v[m + 1 + r];

	for (l = 0, r = 0, I = i; I <= j; ++I)
	{
		if (l < nL && r < nR)
		{
			if ((*CMP)(L[l], R[r]) <= 0)
				v[I] = L[l++];
			else
				v[I] = R[r++];
		}
		else
		{
			if (l >= nL)
				v[I] = R[r++];
			else /* if (r >= nR) or both overrun */
				v[I] = L[l++];
		}
	}
	free(L);
	free(R);
	return;
}

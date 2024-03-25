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
		return;

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
			else if (r >= nR)
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
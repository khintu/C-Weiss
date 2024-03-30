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

/* HeapSort algorithm O(nlgn) */

#define WPARENT(i)	((i) >> 1)	/* i/2 */
#define WLEFT(i)		(((i) << 1) | 0x01) /* 2i + 1 */
#define WRIGHT(i)		(((i) << 1) + 0x02) /* 2i + 2 */
#define WSWAPV(x, y)	{\
												tmp = v[(y)];\
												v[(y)] = v[(x)];\
												v[(x)] = tmp;\
											}

/* MaxHeap property A[PARENT(i)] >= A[i] */
void WMaxHeapifyInt(int v[], int Len, int indx)
{
	int tmp, largest, l, r;
	l = WLEFT(indx);
	r = WRIGHT(indx);
	if (l < Len && v[l] > v[indx])
		largest = l;
	else
		largest = indx;
	if (r < Len && v[r] > v[largest])
		largest = r;
	if (largest != indx)
	{
		WSWAPV(indx, largest);
		WMaxHeapifyInt(v, Len, largest);
	}
	return;
}

/* Convert array from unordered to MAX-HEAP binary tree 
   with each level greater than the subsequent one in value
*/
void WBuildMaxHeapInt(int v[], int Len, int *heapSize)
{
	int i;
	*heapSize = Len;
	for (i = Len/2 - 1; i >= 0; --i)
	{
		WMaxHeapifyInt(v, *heapSize, i);
	}
	return;
}

/* Keep moving root (MAX VALUE) from heap to end of 
   array as you continue to heapify and pop from top 
	 of heap and add in decreasing order to the array
*/
void WHeapSortInt(int v[], int Len)
{
	int tmp, i, heapSize;
	WBuildMaxHeapInt(v, Len, &heapSize);
	for (i = Len - 1; i >= 1 ; --i)
	{
		WSWAPV(0, i);
		heapSize -= 1;
		WMaxHeapifyInt(v, heapSize, 0);
	}
	return;
}

/* MaxHeap property A[PARENT(i)] >= A[i] */
void WMaxHeapify(void* v[], int Len, int indx, int (*CMP)(const void* x, const void* y))
{
	void* tmp;
	int largest, l, r;
	l = WLEFT(indx);
	r = WRIGHT(indx);
	if (l < Len && (*CMP)(v[l], v[indx]) > 0)
		largest = l;
	else
		largest = indx;
	if (r < Len && (*CMP)(v[r], v[largest]) > 0)
		largest = r;
	if (largest != indx)
	{
		WSWAPV(indx, largest);
		WMaxHeapify(v, Len, largest, CMP);
	}
	return;
}

/* Convert array from unordered to MAX-HEAP binary tree
	 with each level greater than the subsequent one in value
*/
void WBuildMaxHeap(void* v[], int Len, int* heapSize, int (*CMP)(const void* x, const void* y))
{
	int i;
	*heapSize = Len;
	for (i = Len / 2 - 1; i >= 0; --i)
	{
		WMaxHeapify(v, *heapSize, i, CMP);
	}
	return;
}

/* Keep moving root (MAX VALUE) from heap to end of
	 array as you continue to heapify and pop from top
	 of heap and add in decreasing order to the array
*/
void WHeapSort(void* v[], int Len, int (*CMP)(const void* x, const void* y))
{
	void* tmp;
	int i, heapSize;
	WBuildMaxHeap(v, Len, &heapSize, CMP); /* O(n) */
	for (i = Len - 1; i >= 1; --i) /* O(n) */
	{
		WSWAPV(0, i);
		heapSize -= 1;
		WMaxHeapify(v, heapSize, 0, CMP); /* O(lgn) */
	}
	return;
}
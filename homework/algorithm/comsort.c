#include<stdio.h>

void _merge(int a[], int p, int q, int r);
void merge_sort(int a[], int p, int r);

int main()
{
  int n, i;
  scanf("%d", &n);
  int a[n];
  for(i = 0; i < n; i++) {
    scanf("%d", &a[i]);
  }
  merge_sort(a, 0, n - 1);
  for(i = 0; i < n; i++) {
    printf("%d ", a[i]);
  }
  printf("\n");
  return 0;
}

void _merge(int a[], int p, int q, int r)
{
  int n1 = q - p + 1;
  int n2 = r - q;
  int i, j, k;
  int L[n1], R[n2];

  for(i = 0; i < n1; i++) {
    L[i] = a[p + i];
  }
  for(j = 0; j < n2; j++) {
    R[j] = a[q + j + 1];
  }

  L[n1] = R[n2] = 1000000;
  i = j = 0;

  for(k = p; k <= r; k++) {
    if(L[i] < R[j]) {
      a[k] = L[i];
      i++;
    }
    else {
      a[k] = R[j];
      j++;
    }
  }
}

void merge_sort(int a[], int p, int r)
{
  int q;
  if(p < r) {
    q = (r + p) / 2;
    merge_sort(a, p, q);
    merge_sort(a, q + 1, r);
    _merge(a, p, q, r);
  }
}

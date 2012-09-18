#include<stdio.h>

void insort(int a[], int n);

int main()
{
  int n, i;
  scanf("%d", &n);
  int a[n];        //in c99
  for(i = 0; i < n; i++) {
    scanf("%d", &a[i]);
  }
  insort(a, n);
  for(i = 0; i < n; i ++) {
    printf("%d ", a[i]);
  }
  printf("\n");
  return 0;
}

void insort(int a[], int n)
{
  int i, j, k, temp;
  for(i = 0; i < n; i++) {
    temp = a[i];
    for(j = 0; j <= i; j++) {
      if(a[i] < a[j]) {
	break;
      }
    }
    for(k = i; k > j; k--) {
      a[k] = a[k - 1];
    }
    if(j <= i)
      a[j] = temp;
  }
}

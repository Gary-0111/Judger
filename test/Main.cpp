/*
* this code is made by 1506200077
* Problem: 1000
* Verdict: Dangerous Code
* Submission Date: 2018-03-10 22:42:12
* Time: -99 MS
* Memory: 1504 KB
*/
#include <stdlib.h>

int a[100005];

int comp(const void *x, const void *y) {
    return *(int*)x - *(int*)y;
}

int main() {
    qsort(a, 100000, sizeof(int), comp);
    return 0;
}
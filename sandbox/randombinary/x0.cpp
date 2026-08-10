#include <cstring>
#include <vector>
#include <iostream>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

const int N = 20;

int n;
int a[N+1], b[N+1];
int f[N+2][N+2];

void relax(int &a, int b) {
	a = max(a, b);
}

int LCS() {
	memset(f, 0, sizeof(f));
	forn(i, n+1)
		forn(j, n+1) {
			relax(f[i + 1][j + 1], f[i][j] + (a[i] == b[j]));
			relax(f[i + 1][j], f[i][j]);
			relax(f[i][j + 1], f[i][j]);
		}
	return f[n][n];
}

void makea(int *a, int mask) {
	forn(i, n)
		a[i] = (mask >> i) & 1;
}

int main() {
	for (n = 2; n <= N; n++) {
		int sum = 0;
		int cnt = 0;
		for (int i = 0; i < (1 << n); i += 2)
			forn(j, 1 << n) {
				makea(a, i);
				makea(b, j);
				sum += LCS();
				cnt++;
			}
		fprintf(stderr, "n = %d : %.6f\n", n, 1. * sum / cnt / n);
		fprintf(stderr, "n = %d : %.6f\n", n, (1. * sum / cnt + 1. / 8) / n);
	}	
}

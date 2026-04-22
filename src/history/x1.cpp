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

pair<int, int> LCS() {
// int LCS() {
	memset(f, 0, sizeof(f));
	forn(i, n+1)
		forn(j, n+1) {
			relax(f[i + 1][j + 1], f[i][j] + (a[i] == b[j]));
			relax(f[i + 1][j], f[i][j]);
			relax(f[i][j + 1], f[i][j]);
		}

	int i1 = n;
	while (i1 > 0 && f[i1-1][n] == f[n][n])
		i1--;
	int value = -1, cnti = 0;
	for (; i1 < n; i1++)
		cnti += (value != a[i1]), value = a[i1];

	int j1 = n;
	while (j1 > 0 && f[n][j1-1] == f[n][n])
		j1--;
	int cntj = 0; value = -1;
	for (; j1 < n; j1++)
		cntj += (value != b[j1]), value = b[j1];

	return {f[n][n], cntj};
}

void makea(int *a, int mask) {
	forn(i, n)
		a[i] = (mask >> i) & 1;
}

int main() {
	for (n = 2; n <= N; n++) {
		double sum = 0;
		int64_t cnt = 0;
		vector<int> cnt_pref(n+1);
		for (int i = 0; i < (1 << n); i += 2)
			forn(j, 1 << n) {
				makea(a, i);
				makea(b, j);
				auto [len,pref] = LCS();
				sum += len;
				cnt_pref[pref]++;
				cnt++;
			}
		// for(int x: cnt_pref)
		// 	printf("%d ", x);
		// puts("");
		auto ans = sum / cnt;
		auto ans2 = ans;
		forn(L1, cnt_pref.size())
			forn(L2, cnt_pref.size()) {
				double E;
				if (!L1 || !L2)
					E = 0;
				else if (L1 == L2)
					E = L1-1 + 0.5;
				else
					E = min(L1, L2);
				ans2 += (double)cnt_pref[L1] * cnt_pref[L2] * E / (1LL << (4 * n));
			}
		ans /= n;
		ans2 /= n;
		printf("n = %d : %.12f : %.12f : %.12f\n", n, ans, 1./8 / n + ans, ans2);
	}	
}

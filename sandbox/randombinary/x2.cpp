#include <vector>
#include <iostream>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

const int N = 6;
const int M = 1 << N;
const int K = 1000;

float mem[K+1][N+1][M][M];

int bit(int x, int i) {
	return (x >> i) & 1;
}

int cnt0 = 0;

double solve(int L, int sh, int ma, int mb) {
	if (L <= 0)
		return 0;
	ma &= (1 << N) - 1;
	mb &= (1 << (N + sh)) - 1;
	auto &r = mem[L][sh][ma][mb >> sh];
	if (r != 0)
		return r - 1;
	if (++cnt0 % int(1e7) == 0)
		fprintf(stderr, "cnt=%d\n", cnt0);
	double sum = 0;
	double cnt = 0;
	// a=[0,N+1] b=[sh,N+1]
	forn(a0, 2)
		forn(b0, 2) {
			int ma1 = ma + (a0 << N);
			int mb1 = mb + (b0 << (N + sh));
			cnt += 1;
			if (bit(ma1, 0) == bit(mb1, sh))
				sum += 1 + solve(L-1, sh, ma1 >> 1, mb1 >> 1);
			else {
				double fa, fb;
				if (sh != 0) 
					fa = solve(L-1, sh-1, ma1 >> 1, mb1 >> 1);
				else
					fa = solve(L-1, 1, mb1, ma1);
				if (sh < N) // [0..N-1]
					fb = solve(L, sh+1, ma1, mb1);
				else 
					fb = solve(L-1, sh, ma1 >> 1, mb1 >> 1);
				sum += max(fa, fb);
			} 
		}
	r = sum / cnt + 1;
	return r - 1;
}

int main() {
	double sum = 0;
	double cnt = 0;
	forn(m1, 1 << N)
		forn(m2, 1 << N) {
			sum += solve(K, 0, m1, m2);
			cnt += 1;
		}
	printf("%.10f\n", sum / cnt / (K+N));
}

#include <vector>
#include <iostream>
#include <cstring>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

using ll = __int128;

const int N = 7, N1 = (1<<N)-1;
const int M = 1 << N;
const int MAX_SH = 30;
const int K = 5000;

float mem[2][MAX_SH+1][M][M];

int bit(ll x, int i) {
	return (x >> i) & 1;
}

int64_t cnt0 = 0;

// double solve(int L, int sh, int ma, int mb) {
// 	if (L <= 0)
// 		return 0;
// 	ma &= (1 << N) - 1;
// 	mb &= (1 << (N + sh)) - 1;
// 	auto &r = mem[L][sh][ma][mb >> sh];
// 	if (r != 0)
// 		return r - 1;
// 	if (++cnt0 % int(1e7) == 0)
// 		fprintf(stderr, "cnt=%d\n", cnt0);
// 	double sum = 0;
// 	double cnt = 0;
// 	// a=[0,N+1] b=[sh,N+1]
// 	forn(a0, 2)
// 		forn(b0, 2) {
// 			int ma1 = ma + (a0 << N);
// 			int mb1 = mb + (b0 << (N + sh));
// 			cnt += 1;
// 			if (bit(ma1, 0) == bit(mb1, sh))
// 				sum += 1 + solve(L-1, sh, ma1 >> 1, mb1 >> 1);
// 			else {
// 				double fa, fb;
// 				if (sh) 
// 					fa = solve(L-1, sh-1, ma1 >> 1, mb1 >> 1);
// 				else
// 					fa = solve(L-1, 1, mb1, ma1);
// 				if (sh < N)
// 					fb = solve(L, sh+1, ma1, mb1);
// 				else 
// 					fb = solve(L-1, sh, ma1 >> 1, mb1 >> 1);
// 				sum += max(fa, fb);
// 			} 
// 		}
// 	r = sum / cnt + 1;
// 	return r - 1;
// }

int main() {
	double sum = 0;
	double cnt = 0;
	for (int L = 1; L <= K; L++) {
		auto m0 = mem[L & 1];
		auto m1 = mem[(L & 1) ^ 1];
		memset(m0, 0, sizeof(mem[0]));
		for (int sh = MAX_SH; sh >= 0; sh--)
			forn(ma, 1 << N)
				forn(mb, 1 << N) {
					if (++cnt0 % int(1e7) == 0)
						fprintf(stderr, "cnt=%lld\n", cnt0);
					double sum = 0;
					double cnt = 0;
					// a=[0,N+1] b=[sh,N+1]
					forn(a0, 2)
						forn(b0, 2) {
							ll ma1 = ma + (a0 << N);
							ll mb1 = ((ll)mb << sh) + ((ll)b0 << (N + sh));
							cnt += 1;
							if (bit(ma1, 0) == bit(mb1, sh))
								sum += 1 + m1[sh][ma1 >> 1][mb1 >> (1 + sh)];
							else {
								double fa, fb;
								if (sh) 
									fa = m1[sh-1][ma1 >> 1][(mb1 >> sh) & N1];
								else
									fa = m1[1][mb1 & N1][ma1 >> 1];
								if (sh < MAX_SH)
									fb = m0[sh+1][ma1 & N1][mb1 >> (sh+1)];
								else 
									fb = m1[sh][ma1 >> 1][mb1 >> (1 + sh)];
								sum += max(fa, fb);
							} 
						}
					m0[sh][ma][mb] = sum / cnt;
				}
	}
	forn(m1, 1 << N)
		forn(m2, 1 << N) {
			sum += mem[K & 1][0][m1][m2];
			cnt += 1;
		}
	printf("%.10f\n", sum / cnt / (K+MAX_SH));
}

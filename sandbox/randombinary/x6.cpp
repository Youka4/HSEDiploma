#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("tune=native")
#pragma GCC target("avx2")
	

#include <vector>
#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

// change N, MAX_SH, K (SH=shift)
// space = 4*2^{2N}*MAX_SH bytes
// time = Theta(2^{2N}*MAX_SH*K)
const int N = 9, N1 = (1<<N)-1, M = 1 << N;
const int MAX_SH = 30;
const int K = 1e5;

double m[MAX_SH+1][M][M];
double m0[M][M];
double m1[M][M];

int bit(int x, int i) {
	return (x >> i) & 1;
}

int cnt0 = 0, step0 = 0;

int main() {
	for (int L = 1; L <= K; L++) {
		memcpy(m0, m[1], sizeof(m0));
		for (int sh = MAX_SH; sh >= 0; sh--) {
			memcpy(m1, m[sh], sizeof(m1));
			forn(ma, 1 << N)
				forn(mb, 1 << N) {
					if ((++cnt0 & ((1 << 25) - 1)) == 0)
						printf("[%.5f]", 1.*L/K), cnt0 = 0;
					double sum = 0;
					int cnt = 0;
					// a=[0,N+1] b=[sh,N+1]
					forn(a0, 2)
						forn(b0, 2) {
							int ma1 = ma + (a0 << N);
							int mb1 = mb + (b0 << N);
							cnt += 1;
							if (bit(ma1, 0) == bit(mb1, 0))
								sum += 1 + m1[ma1 >> 1][mb1 >> 1];
							else {
								double fa, fb;
								if (sh) 
									fa = m[sh-1][ma1 >> 1][mb1 & N1];
								else
									fa = m0[mb1 & N1][ma1 >> 1];
								if (sh < MAX_SH)
									fb = m[sh+1][ma1 & N1][mb1 >> 1];
								else 
									fb = m1[ma1 >> 1][mb1 >> 1];
								sum += max(fa, fb);
							} 
						}
					m[sh][ma][mb] = sum / cnt;
				}
		}

		double sum = 0;
		double cnt = 0;
		forn(ma, 1 << N)
			forn(mb, 1 << N) {
				sum += m[0][ma][mb];
				cnt += 1;
			}
		printf("[%d|ans=%.5f]\n", L, (double)sum / cnt / (L+MAX_SH));
	}

	double sum = 0;
	double cnt = 0;
	forn(ma, 1 << N)
		forn(mb, 1 << N) {
			sum += m[0][ma][mb];
			cnt += 1;
		}
	printf("!\n"), cnt0 = 0;
	printf("%.10f [N=%d, SH=%d, K=%d]\n", (double)sum / cnt / (K+MAX_SH), N, MAX_SH, K);
}

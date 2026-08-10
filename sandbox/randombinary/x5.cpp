#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("tune=native")
// #pragma GCC target("avx2") // can not be combined with vector of array
	
#include <vector>
#include <iostream>
#include <cstring>
#include <cmath>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

// change N, MAX_SH, K (SH=shift)
// space = 4*2^{2N}*MAX_SH bytes
// time = Theta(2^{2N}*MAX_SH*K)
const int N = 12, N1 = (1<<N)-1, M = 1 << N;
const int MAX_SH = 30; // [0...MAX_SH]
const int K = 40000;

struct F {
	float a[M][M];
};

vector<F> m(MAX_SH+1);
float m0[M][M];
float m1[M][M];

int bit(int x, int i
	) {
	return (x >> i) & 1;
}

int cnt0 = 0, step0 = 0;

int main() {
	for (int L = 1; L <= K; L++) {
		memcpy(m0, m[1].a, sizeof(m0));
		for (int sh = MAX_SH; sh >= 0; sh--) {
			// m1 = m[sh]
			memcpy(m1, m[sh].a, sizeof(m1));
			forn(ma, 1 << N)
				forn(mb, 1 << N) {
					if ((++cnt0 & ((1 << 25) - 1)) == 0)
						printf("[%.3f%%]", 100.*L/K), cnt0 = 0;
					float sum = 0;
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
								float fa, fb;
								// A [x.........]
								// B       [y...........]
								if (sh != 0) 
									fa = m[sh-1].a[ma1 >> 1][mb1 & N1];
								else
									fa = m0[mb1 & N1][ma1 >> 1];
								if (sh < MAX_SH)
									fb = m[sh+1].a[ma1 & N1][mb1 >> 1];
								else 
									// fb = -1;
									fb = m1[ma1 >> 1][mb1 >> 1];
								sum += max(fa, fb);
							} 
						}
					m[sh].a[ma][mb] = sum / cnt;
				}
		}

		float sum = 0;
		float cnt = 0;
		forn(ma, 1 << N)
			forn(mb, 1 << N) {
				sum += m[0].a[ma][mb];
				cnt += 1;
			}
		printf("[%d|ans=%.5f]\n", L, (double)sum / cnt / (L+MAX_SH));
	}

	float sum = 0;
	float cnt = 0;
	forn(ma, 1 << N)
		forn(mb, 1 << N) {
			sum += m[0].a[ma][mb];
			cnt += 1;
		}
	printf("!\n"), cnt0 = 0;
	printf("%.10f [N=%d, SH=%d, K=%d]\n", (double)sum / cnt / (K+MAX_SH), N, MAX_SH, K);
	fprintf(stderr, "time = %.3f\n", 1. * clock() / CLOCKS_PER_SEC);
}

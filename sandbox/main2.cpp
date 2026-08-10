
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

const int N = 5;
const int MAX_SH = 6;
const int L = 1e4;

int main() {
}

float dp[L+1][MAX_SH+ 1][1<<N][1<<N];
int main() {
    //  dp[L][sh][maskA][maskB]
    for (int l = 1; l <= L; l++) {
        for (int sh = MAX_SH; sh >= 0; sh--) {
            for (int maskA = 0; maskA < (1<<N); maskA++) {
                for (int maskB = 0; maskB < (1<<N); maskB++) {
                    float res = 0;
                    for (int nextA = 0; nextA < 2; nextA++) {
                        for (int nextB = 0; nextB < 2; nextB++) {
                            int nextMaskA = (maskA >> 1) | (nextA << (N-1));
                            int nextMaskB = (maskB >> 1) | (nextB << (N-1));
                            float tmp_res = 0;
                            if ((maskA & 1) == (maskB & 1)) {
                                tmp_res = 1 + dp[l-1][sh][nextMaskA][nextMaskB];
                            } else {
                                 if (sh > 0){
                                    tmp_res = dp[l-1][sh-1][nextMaskA][maskB];
                                 } else {
                                    tmp_res = dp[l][sh+1][maskB][nextMaskA];
                                 }
                                 if (sh < MAX_SH) {
                                    tmp_res = max(tmp_res, dp[l][sh+1][maskA][nextMaskB]);
                                 } else {
                                     tmp_res = max(tmp_res, dp[l-1][sh][nextMaskA][nextMaskB]);
                                 }
                            }
                            res += tmp_res;
                        }
                    }
                    res /= 4;
                    dp[l][sh][maskA][maskB] = res;
                }
            }
        }
    }
    float ans = 0;
    int cnt = 0;
    for (int maskA = 0; maskA < (1<<N); maskA++) {
        for (int maskB = 0; maskB < (1<<N); maskB++) {
            ans += dp[L][0][maskA][maskB];
            cnt++;
        }
    }
    ans /= (1<<(2*N));
    printf("%.10f %.10f [N=%d, SH=%d, K=%d]\n", (double)ans / L, (double)ans / (L+ MAX_SH), N, MAX_SH, L);
}
// 0.7762332031 0.7757677425 [N=5, SH=6, K=10000]
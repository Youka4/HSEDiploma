
#include <iomanip>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

const int N = 5;
const int MAX_SH = 6;
const int L = 1e4;

float dp[MAX_SH+1][1<<N][1<<N];
float dp_temp[1<<N][1<<N];
float dp_one_sh[1<<N][1<<N];
float dp_plus_one_sh[1<<N][1<<N];
int main() {
    //  dp[L][sh][maskA][maskB] -> dp[sh][maskA][maskB]
    for (int l = 1; l <= L; l++) {
        memcpy(dp_one_sh, dp[1], sizeof(dp_one_sh));
        for (int sh = MAX_SH; sh >= 0; sh--) {
            memcpy(dp_temp, dp[sh], sizeof(dp_temp));
            if (sh < MAX_SH) {
                memcpy(dp_plus_one_sh, dp[sh+1], sizeof(dp_plus_one_sh));
            }
            for (int maskA = 0; maskA < (1<<N); maskA++) {
                for (int maskB = 0; maskB < (1<<N); maskB++) {
                    float res = 0;
                    for (int nextA = 0; nextA < 2; nextA++) {
                        for (int nextB = 0; nextB < 2; nextB++) {
                            int nextMaskA = (maskA >> 1) | (nextA << (N-1));
                            int nextMaskB = (maskB >> 1) | (nextB << (N-1));
                            float tmp_res = 0;
                            if ((maskA & 1) == (maskB & 1)) {
                                tmp_res = 1 + dp_temp[nextMaskA][nextMaskB];
                            } else {
                                 if (sh > 0){
                                    tmp_res = dp[sh-1][nextMaskA][maskB];
                                 } else {
                                    tmp_res = dp_one_sh[maskB][nextMaskA];
                                 }
                                 if (sh < MAX_SH) {
                                    tmp_res = max(tmp_res, dp_plus_one_sh[maskA][nextMaskB]);
                                 } else {
                                     tmp_res = max(tmp_res, dp_temp[nextMaskA][nextMaskB]);
                                 }
                            }
                            res += tmp_res;
                        }
                    }
                    res /= 4;
                    dp[sh][maskA][maskB] = res;
                }
            }
        }
    }
    float ans = 0;
    int cnt = 0;
    for (int maskA = 0; maskA < (1<<N); maskA++) {
        for (int maskB = 0; maskB < (1<<N); maskB++) {
            ans += dp[0][maskA][maskB];
            cnt++;
        }
    }
    ans /= (1<<(2*N));
    printf("%.10f %.10f [N=%d, SH=%d, K=%d]\n", (double)ans / L, (double)ans / (L+ MAX_SH), N, MAX_SH, L);
}
// 0.7762332031 0.7757677425 [N=5, SH=6, K=10000]
// added dp_one_sh 0.7679438965 0.7674834064 [N=5, SH=6, K=10000]
// added dp_plus_one_sh nothing changed
#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("tune=native")
#pragma GCC target("avx2")

#include <iostream>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

// ============================================================
// Универсальное оконное ДП для константы Чватала-Санкоффа
// Работает для произвольного размера алфавита SIGMA.
//
// Кодирование: каждый символ = BITS бит, где BITS = ceil(log2(SIGMA)).
// Маска из N символов = BITS*N бит, M = 2^(BITS*N) значений масок.
// При SIGMA не степени двойки, маски с «нелегальными» символами
// (>=SIGMA) никогда не генерируются и остаются нулевыми.
//
// Оптимизация: предвычисляем список валидных масок (все символы < SIGMA)
// и итерируем только по ним. Для SIGMA=9, N=3: 729 вместо 4096.
//
// Параметры задаются через аргументы командной строки:
//   ./windowed_dp_universal <SIGMA> <N> <MAX_SH> <K>
//
// Память: O(MAX_SH * M * M) float
// Время:  O(K * MAX_SH * VM * VM * SIGMA^2) где VM = SIGMA^N
// ============================================================

// Глобальные параметры (устанавливаются из argv)
int SIGMA;
int BITS;
int N_WIN;       // размер окна
int MASK_BITS;   // BITS * N_WIN
int M;           // 2^MASK_BITS
int SYM_MASK;    // (1 << BITS) - 1
int FULL_MASK;   // M - 1
int MAX_SH;
int K;

// Массивы выделяем динамически из-за переменных размеров
float *dp;       // [MAX_SH+1][M][M]
float *dp0;      // [M][M]  — копия dp[1]
float *dp1;      // [M][M]  — копия dp[sh] (текущий слой)

inline float& D(int sh, int ma, int mb) {
    return dp[(long long)sh * M * M + (long long)ma * M + mb];
}
inline float& D0(int ma, int mb) {
    return dp0[(long long)ma * M + mb];
}
inline float& D1(int ma, int mb) {
    return dp1[(long long)ma * M + mb];
}

// Проверка, что все символы маски < SIGMA
bool is_valid_mask(int mask) {
    for (int i = 0; i < N_WIN; i++) {
        int sym = (mask >> (BITS * i)) & SYM_MASK;
        if (sym >= SIGMA) return false;
    }
    return true;
}

int cnt0 = 0;

int main(int argc, char* argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s <SIGMA> <N> <MAX_SH> <K>\n", argv[0]);
        fprintf(stderr, "  SIGMA  — alphabet size (2..16)\n");
        fprintf(stderr, "  N      — window size (visible symbols)\n");
        fprintf(stderr, "  MAX_SH — max shift [0..MAX_SH]\n");
        fprintf(stderr, "  K      — string length (iterations)\n");
        return 1;
    }

    SIGMA = atoi(argv[1]);
    N_WIN = atoi(argv[2]);
    MAX_SH = atoi(argv[3]);
    K = atoi(argv[4]);

    // Вычисляем BITS = ceil(log2(SIGMA))
    BITS = 1;
    while ((1 << BITS) < SIGMA) BITS++;

    MASK_BITS = BITS * N_WIN;
    if (MASK_BITS > 24) {
        fprintf(stderr, "ERROR: MASK_BITS=%d too large (max 24), reduce N or use smaller alphabet\n", MASK_BITS);
        return 1;
    }
    M = 1 << MASK_BITS;
    SYM_MASK = (1 << BITS) - 1;
    FULL_MASK = M - 1;

    // Предвычисляем валидные маски
    vector<int> valid_masks;
    bool is_power_of_2 = (SIGMA & (SIGMA - 1)) == 0;
    if (is_power_of_2) {
        // Все маски валидны
        valid_masks.resize(M);
        forn(i, M) valid_masks[i] = i;
    } else {
        for (int m = 0; m < M; m++) {
            if (is_valid_mask(m)) valid_masks.push_back(m);
        }
    }
    int VM = valid_masks.size();

    long long total_mem = (long long)(MAX_SH + 1 + 2) * M * M * sizeof(float);
    fprintf(stderr, "SIGMA=%d, BITS=%d, N=%d, MASK_BITS=%d, M=%d\n", SIGMA, BITS, N_WIN, MASK_BITS, M);
    fprintf(stderr, "Valid masks: %d (out of %d total, ratio %.2fx)\n", VM, M, (double)M / VM);
    fprintf(stderr, "MAX_SH=%d, K=%d\n", MAX_SH, K);
    fprintf(stderr, "Memory: %.2f MB\n", total_mem / 1e6);

    dp  = (float*)calloc((long long)(MAX_SH + 1) * M * M, sizeof(float));
    dp0 = (float*)calloc((long long)M * M, sizeof(float));
    dp1 = (float*)calloc((long long)M * M, sizeof(float));

    if (!dp || !dp0 || !dp1) {
        fprintf(stderr, "ERROR: failed to allocate %.2f MB\n", total_mem / 1e6);
        return 1;
    }

    for (int L = 1; L <= K; L++) {
        memcpy(dp0, dp + (long long)1 * M * M, (long long)M * M * sizeof(float));

        for (int sh = MAX_SH; sh >= 0; sh--) {
            memcpy(dp1, dp + (long long)sh * M * M, (long long)M * M * sizeof(float));

            for (int ia = 0; ia < VM; ia++) {
                int ma = valid_masks[ia];
                for (int ib = 0; ib < VM; ib++) {
                    int mb = valid_masks[ib];

                    if ((++cnt0 & ((1 << 25) - 1)) == 0)
                        fprintf(stderr, "[%.5f]", 1.0 * L / K), cnt0 = 0;

                    float sum = 0;
                    int cnt = 0;

                    forn(a0, SIGMA)
                        forn(b0, SIGMA) {
                            int ma1 = (ma >> BITS) | (a0 << (MASK_BITS - BITS));
                            int mb1 = (mb >> BITS) | (b0 << (MASK_BITS - BITS));
                            cnt += 1;

                            if ((ma & SYM_MASK) == (mb & SYM_MASK)) {
                                sum += 1 + D1(ma1, mb1);
                            } else {
                                float fa, fb = 0;
                                if (sh != 0)
                                    fa = D(sh - 1, ma1, mb & FULL_MASK);
                                else
                                    fa = D0(mb & FULL_MASK, ma1);
                                if (sh < MAX_SH)
                                    fb = D(sh + 1, ma & FULL_MASK, mb1);
                                else
                                    fb = D1(ma1, mb1);
                                sum += max(fa, fb);
                            }
                        }
                    D(sh, ma, mb) = sum / cnt;
                }
            }
        }
    }

    double sum = 0;
    double cnt = 0;
    for (int ia = 0; ia < VM; ia++)
        for (int ib = 0; ib < VM; ib++) {
            sum += D(0, valid_masks[ia], valid_masks[ib]);
            cnt += 1;
        }
    fprintf(stderr, "!\n");
    printf("%.10f [SIGMA=%d, N=%d, SH=%d, K=%d]\n",
           sum / cnt / (K + MAX_SH), SIGMA, N_WIN, MAX_SH, K);

    free(dp);
    free(dp0);
    free(dp1);
    return 0;
}

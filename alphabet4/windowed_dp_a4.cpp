#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")
#pragma GCC target("tune=native")
#pragma GCC target("avx2")

#include <iostream>
#include <cstring>
#include <algorithm>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

// ============================================================
// Оконное ДП для константы Чватала-Санкоффа, размер алфавита SIGMA=4
// Каждый символ занимает 2 бита (значения 0,1,2,3)
// Маска из N символов = 2*N бит, всего M = 4^N значений масок
//
// Настраиваемые параметры: N, MAX_SH, K
// Память: O(MAX_SH * M * M) float = MAX_SH * 16^N * 4 байт
// Время:  O(K * MAX_SH * M * M * SIGMA^2) операций
//
// N=5:  M=1024, M*M=1 048 576 пар, 16 комбинаций на пару
// N=6:  M=4096, M*M=16 777 216 — нужно ~2.5 ГБ на слой сдвига
// ============================================================

const int SIGMA = 4;       // размер алфавита
const int BITS = 2;        // бит на символ (log2(SIGMA))
const int N = 5;           // размер окна (число видимых символов)
const int MASK_BITS = BITS * N;          // всего бит в маске
const int M = 1 << MASK_BITS;           // 4^N = число различных масок
const int SYM_MASK = SIGMA - 1;         // маска одного символа: 0b11
const int FULL_MASK = M - 1;            // маска N символов
const int MAX_SH = 15;    // максимальный сдвиг [0..MAX_SH]
const int K = 5000;        // длина строк (число итераций)

float m[MAX_SH + 1][M][M];
float m0[M][M];     // копия m[1] (для трюка с обменом при sh=0)
float m1[M][M];     // копия m[sh] (текущий слой)

// Извлечь i-й символ (нумерация с 0, от младших бит) из маски x
inline int sym(int x, int i) {
    return (x >> (BITS * i)) & SYM_MASK;
}

int cnt0 = 0;

int main() {
    fprintf(stderr, "Alphabet=%d, N=%d, MAX_SH=%d, K=%d\n", SIGMA, N, MAX_SH, K);
    fprintf(stderr, "Mask values per string: %d (4^%d)\n", M, N);
    fprintf(stderr, "State space per L: %d * %d * %d = %.2e\n",
            MAX_SH + 1, M, M, (double)(MAX_SH + 1) * M * M);

    for (int L = 1; L <= K; L++) {
        memcpy(m0, m[1], sizeof(m0));
        for (int sh = MAX_SH; sh >= 0; sh--) {
            memcpy(m1, m[sh], sizeof(m1));
            forn(ma, M)
                forn(mb, M) {
                    if ((++cnt0 & ((1 << 25) - 1)) == 0)
                        fprintf(stderr, "[%.5f]", 1.0 * L / K), cnt0 = 0;

                    float sum = 0;
                    int cnt = 0;

                    // Перебираем все возможные новые символы для A и B
                    // Каждый новый символ из {0,1,2,3}
                    forn(a0, SIGMA)
                        forn(b0, SIGMA) {
                            // Сдвигаем маску вправо на 1 символ (отбрасываем младший),
                            // новый символ ставим в старшую позицию
                            int ma1 = (ma >> BITS) | (a0 << (MASK_BITS - BITS));
                            int mb1 = (mb >> BITS) | (b0 << (MASK_BITS - BITS));
                            cnt += 1;

                            // Проверяем совпадение ведущих символов
                            // Ведущий символ = младшие BITS бит
                            if ((ma & SYM_MASK) == (mb & SYM_MASK)) {
                                // Берём символ в НОП
                                sum += 1 + m1[ma1][mb1];
                            } else {
                                float fa, fb = 0;
                                // Вариант 1: продвинуть A (уменьшить сдвиг)
                                if (sh != 0)
                                    fa = m[sh - 1][ma1][mb & FULL_MASK];
                                else
                                    fa = m0[mb & FULL_MASK][ma1];
                                // Вариант 2: продвинуть B (увеличить сдвиг)
                                if (sh < MAX_SH)
                                    fb = m[sh + 1][ma & FULL_MASK][mb1];
                                else
                                    fb = m1[ma1][mb1];
                                sum += max(fa, fb);
                            }
                        }
                    m[sh][ma][mb] = sum / cnt;
                }
        }
    }

    // Вычисляем среднее по всем парам масок при сдвиге=0
    double sum = 0;
    double cnt = 0;
    forn(ma, M)
        forn(mb, M) {
            sum += m[0][ma][mb];
            cnt += 1;
        }
    fprintf(stderr, "!\n");
    printf("%.10f [SIGMA=%d, N=%d, SH=%d, K=%d]\n",
           sum / cnt / (K + MAX_SH), SIGMA, N, MAX_SH, K);
    return 0;
}

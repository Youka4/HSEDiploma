#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>
#include <cstring>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

mt19937 rng(time(0));

int compute_lcs(const vector<int>& a, const vector<int>& b) {
    int n = a.size(), m = b.size();
    vector<int> prev(m + 1, 0), curr(m + 1, 0);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i-1] == b[j-1]) curr[j] = prev[j-1] + 1;
            else curr[j] = max(prev[j], curr[j-1]);
        }
        swap(prev, curr);
        fill(curr.begin(), curr.end(), 0);
    }
    return prev[m];
}

int main(int argc, char* argv[]) {
    int SIGMA = 2;
    int STRING_LENGTH = 10000;
    int NUM_TRIALS = 500;
    bool quiet_mode = false;
    unsigned int seed = time(0);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-k") == 0 && i + 1 < argc) {
            SIGMA = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            STRING_LENGTH = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            NUM_TRIALS = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-q") == 0) {
            quiet_mode = true;
        } else if (strcmp(argv[i], "-h") == 0) {
            printf("Usage: %s [-k sigma] [-n length] [-t trials] [-s seed] [-q]\n", argv[0]);
            return 0;
        }
    }

    rng.seed(seed);
    uniform_int_distribution<int> dist(0, SIGMA - 1);

    if (!quiet_mode) {
        printf("Monte Carlo LCS, SIGMA=%d, n=%d, trials=%d, seed=%u\n",
               SIGMA, STRING_LENGTH, NUM_TRIALS, seed);
    }

    long long total_lcs = 0;
    long long total_lcs_sq = 0;
    int min_lcs = INT_MAX, max_lcs = 0;
    auto start = clock();

    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        vector<int> a(STRING_LENGTH), b(STRING_LENGTH);
        for (int i = 0; i < STRING_LENGTH; i++) { a[i] = dist(rng); b[i] = dist(rng); }
        int lcs = compute_lcs(a, b);
        total_lcs += lcs;
        total_lcs_sq += (long long)lcs * lcs;
        min_lcs = min(min_lcs, lcs);
        max_lcs = max(max_lcs, lcs);
        if (!quiet_mode && (trial + 1) % 50 == 0) {
            double avg = (double)total_lcs / (trial + 1);
            printf("  [%d/%d] avg=%.2f ratio=%.6f\n", trial+1, NUM_TRIALS, avg, avg/STRING_LENGTH);
        }
    }

    double elapsed = (double)(clock() - start) / CLOCKS_PER_SEC;
    double avg = (double)total_lcs / NUM_TRIALS;
    double var = (double)total_lcs_sq / NUM_TRIALS - avg * avg;
    double ratio = avg / STRING_LENGTH;

    printf("\nSIGMA=%d n=%d trials=%d E[LCS]=%.2f E[LCS]/n=%.6f std=%.2f time=%.1fs\n",
           SIGMA, STRING_LENGTH, NUM_TRIALS, avg, ratio, sqrt(var), elapsed);

    return 0;
}

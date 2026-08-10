#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

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

int main() {
    mt19937 rng(42);
    int n = 5000, trials = 200;

    for (int sigma = 2; sigma <= 8; sigma++) {
        long long total = 0;
        for (int t = 0; t < trials; t++) {
            vector<int> a(n), b(n);
            uniform_int_distribution<int> dist(0, sigma - 1);
            for (int i = 0; i < n; i++) { a[i] = dist(rng); b[i] = dist(rng); }
            total += compute_lcs(a, b);
        }
        double ratio = (double)total / trials / n;
        printf("sigma=%d: E[LCS]/n = %.6f\n", sigma, ratio);
    }
}

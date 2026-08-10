#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

const int N = 1000;

double C(int n, int k) {
	double r = 0;
	forn(i, k)
		r += log(n - i), r -= log(i + 1);
		// r *= n - i, r /= i + 1;
	return r;
}

double sqr(double x) {
	return x + x;
	// return x * x;
}

double Pow(double x, double y) {
	return log(x) * y;
}

int main() {
	for (int n = 2; n <= N; n++) {
		int k = n * 0.05;
		// for (int k = 1; k * 2 <= n; k++)
		auto A = Pow(2, 2 * n);
		auto B = Pow(2, n) + sqr(C(n, k));  
		// pow(2., 2. * n),
		// pow(2., 1. * n) * sqr(C(n, k)));
		printf("n = %d, k = %d, all_LCS = %g, max = %g : %g\n", n, k, A, B, A / B);
	}
}

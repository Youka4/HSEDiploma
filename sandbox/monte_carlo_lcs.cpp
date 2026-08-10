#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>

using namespace std;

// Генератор случайных чисел
mt19937 rng(time(0));

// Генерация случайной бинарной строки
string generate_random_binary_string(int length) {
    string s;
    s.reserve(length);
    uniform_int_distribution<int> dist(0, 1);
    for (int i = 0; i < length; i++) {
        s += '0' + dist(rng);
    }
    return s;
}

// Вычисление LCS с оптимизацией памяти O(n)
int compute_lcs(const string& a, const string& b) {
    int n = a.length();
    int m = b.length();

    // Используем только две строки вместо всей матрицы
    vector<int> prev(m + 1, 0);
    vector<int> curr(m + 1, 0);

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i-1] == b[j-1]) {
                curr[j] = prev[j-1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j-1]);
            }
        }
        swap(prev, curr);
        fill(curr.begin(), curr.end(), 0);
    }

    return prev[m];
}

int main() {
    const int STRING_LENGTH = 10000;
    const int NUM_TRIALS = 1000;

    cout << "Метод Монте-Карло для LCS двух случайных бинарных строк" << endl;
    cout << "Длина строк: " << STRING_LENGTH << endl;
    cout << "Количество испытаний: " << NUM_TRIALS << endl;
    cout << "=" << string(60, '=') << endl;

    long long total_lcs = 0;
    int min_lcs = INT_MAX;
    int max_lcs = 0;

    auto start_time = clock();

    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        // Генерируем две случайные бинарные строки
        string a = generate_random_binary_string(STRING_LENGTH);
        string b = generate_random_binary_string(STRING_LENGTH);

        // Вычисляем LCS
        int lcs = compute_lcs(a, b);

        total_lcs += lcs;
        min_lcs = min(min_lcs, lcs);
        max_lcs = max(max_lcs, lcs);

        // Прогресс
        if ((trial + 1) % 100 == 0) {
            double avg_so_far = (double)total_lcs / (trial + 1);
            double progress = 100.0 * (trial + 1) / NUM_TRIALS;
            cout << "Прогресс: " << fixed << setprecision(1) << progress << "% "
                 << "| Текущее среднее: " << setprecision(2) << avg_so_far
                 << " | LCS[" << trial + 1 << "] = " << lcs << endl;
        }
    }

    auto end_time = clock();
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    double average_lcs = (double)total_lcs / NUM_TRIALS;
    double ratio = average_lcs / STRING_LENGTH;

    cout << "\n" << string(60, '=') << endl;
    cout << "РЕЗУЛЬТАТЫ:" << endl;
    cout << string(60, '=') << endl;
    cout << "Среднее значение LCS: " << fixed << setprecision(4) << average_lcs << endl;
    cout << "Минимум: " << min_lcs << endl;
    cout << "Максимум: " << max_lcs << endl;
    cout << "Отношение E[LCS]/n: " << setprecision(6) << ratio << endl;
    cout << "Время выполнения: " << setprecision(2) << elapsed << " сек" << endl;
    cout << "Среднее время на один LCS: " << (elapsed / NUM_TRIALS * 1000) << " мс" << endl;

    // Теоретическое значение для случайных бинарных строк
    // Для алфавита размера 2: E[LCS]/n ≈ 2/3 (примерно)
    cout << "\nПримечание: для случайных бинарных строк теоретически E[LCS]/n ≈ 0.666..." << endl;

    return 0;
}

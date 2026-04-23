#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <iomanip>
#include <cstring>

using namespace std;

const int SIGMA = 4; // размер алфавита

mt19937 rng(time(0));

// Генерация случайной строки над алфавитом {0,1,...,SIGMA-1}
string generate_random_string(int length) {
    string s;
    s.reserve(length);
    uniform_int_distribution<int> dist(0, SIGMA - 1);
    for (int i = 0; i < length; i++) {
        s += (char)('0' + dist(rng));
    }
    return s;
}

// НОП с O(n) памяти
int compute_lcs(const string& a, const string& b) {
    int n = a.length();
    int m_len = b.length();

    vector<int> prev(m_len + 1, 0);
    vector<int> curr(m_len + 1, 0);

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m_len; j++) {
            if (a[i - 1] == b[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        swap(prev, curr);
        fill(curr.begin(), curr.end(), 0);
    }

    return prev[m_len];
}

void print_usage(const char* program_name) {
    cout << "Использование: " << program_name << " [опции]" << endl;
    cout << "Монте-Карло НОП для случайных строк над алфавитом {0,1,2,3}" << endl;
    cout << "Опции:" << endl;
    cout << "  -n <int>   Длина строк (по умолчанию: 10000)" << endl;
    cout << "  -t <int>   Число испытаний (по умолчанию: 1000)" << endl;
    cout << "  -s <int>   Seed для ГСЧ" << endl;
    cout << "  -q         Тихий режим" << endl;
    cout << "  -h         Показать справку" << endl;
}

int main(int argc, char* argv[]) {
    int STRING_LENGTH = 10000;
    int NUM_TRIALS = 1000;
    bool quiet_mode = false;
    unsigned int seed = time(0);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            STRING_LENGTH = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
            NUM_TRIALS = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-q") == 0) {
            quiet_mode = true;
        } else if (strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        }
    }

    rng.seed(seed);

    if (!quiet_mode) {
        cout << "Монте-Карло НОП, размер алфавита = " << SIGMA << endl;
        cout << "Длина строк: " << STRING_LENGTH << endl;
        cout << "Испытаний: " << NUM_TRIALS << endl;
        cout << "Seed: " << seed << endl;
        cout << string(70, '=') << endl;
    }

    long long total_lcs = 0;
    long long total_lcs_squared = 0;
    int min_lcs = INT_MAX;
    int max_lcs = 0;

    auto start_time = clock();

    for (int trial = 0; trial < NUM_TRIALS; trial++) {
        string a = generate_random_string(STRING_LENGTH);
        string b = generate_random_string(STRING_LENGTH);

        int lcs = compute_lcs(a, b);

        total_lcs += lcs;
        total_lcs_squared += (long long)lcs * lcs;
        min_lcs = min(min_lcs, lcs);
        max_lcs = max(max_lcs, lcs);

        if (!quiet_mode && (trial + 1) % 100 == 0) {
            double avg_so_far = (double)total_lcs / (trial + 1);
            double progress = 100.0 * (trial + 1) / NUM_TRIALS;
            cout << "Прогресс: " << fixed << setprecision(1) << progress << "% "
                 << "| Среднее: " << setprecision(2) << avg_so_far
                 << " | НОП[" << trial + 1 << "] = " << lcs << endl;
        }
    }

    auto end_time = clock();
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    double average_lcs = (double)total_lcs / NUM_TRIALS;
    double variance = (double)total_lcs_squared / NUM_TRIALS - average_lcs * average_lcs;
    double std_dev = sqrt(variance);
    double ratio = average_lcs / STRING_LENGTH;

    if (!quiet_mode) {
        cout << "\n" << string(70, '=') << endl;
    }
    cout << "РЕЗУЛЬТАТЫ (SIGMA=" << SIGMA << "):" << endl;
    cout << string(70, '=') << endl;
    cout << "Параметры: n=" << STRING_LENGTH << ", испытаний=" << NUM_TRIALS << endl;
    cout << "Среднее НОП: " << fixed << setprecision(4) << average_lcs << endl;
    cout << "Ст. откл.: " << setprecision(4) << std_dev << endl;
    cout << "Мин: " << min_lcs << endl;
    cout << "Макс: " << max_lcs << endl;
    cout << "E[НОП]/n: " << setprecision(6) << ratio << endl;

    if (!quiet_mode) {
        cout << "\nВремя: " << setprecision(2) << elapsed << " сек" << endl;
        cout << "Среднее на испытание: " << (elapsed / NUM_TRIALS * 1000) << " мс" << endl;
    }

    cout << "\nИтог: "
         << "SIGMA=" << SIGMA
         << " n=" << STRING_LENGTH
         << " испытаний=" << NUM_TRIALS
         << " E[НОП]=" << setprecision(2) << average_lcs
         << " E[НОП]/n=" << setprecision(6) << ratio
         << " s=" << setprecision(2) << std_dev << endl;

    return 0;
}

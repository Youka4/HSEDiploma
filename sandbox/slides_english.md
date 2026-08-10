# Slides — English Translation

---

## Slide 1 (Title)

**Improved Lower Bound for the Chvátal–Sankoff Constant via Windowed Dynamic Programming**

School of Informatics, Physics and Technology
Applied Mathematics and Computer Science
Saint Petersburg, 2025
Dmitry Yukachev, 4th year student

---

## Slide 2 — Introduction to the Problem Domain

c

**Longest Common Subsequence (LCS)** — the longest common subsequence of two strings. A subsequence is obtained by deleting some elements without changing the order of the remaining ones.

**Random binary strings** — two independent strings X, Y of length n, where each element is independently drawn from {0, 1} with probability 1/2.

**The Chvátal–Sankoff problem (1975):** For two random binary strings of length n, the expected length of their LCS is asymptotically linear:

E[L_n] ~ γ_2 · n as n → ∞

where γ_2 is the Chvátal–Sankoff constant for the binary alphabet.

**Open problem:** The exact value of γ_2 has been unknown since 1975!

**Current bounds:**
- Lower: γ_2 ≥ 0.792666 (Heineman et al., 2024)
- Upper: γ_2 ≤ 0.826280 (Lueker, 2009)
- Expected: γ_2 ~ 0.811 (Monte Carlo method)

*Example:*
LCS("ABCD", "ACBD") = "ABD" (length 3)

---

## Slide 3 — Research Goal and Objectives

**Goal:**

Obtain an improved rigorous lower bound for the Chvátal–Sankoff constant γ_2, surpassing the current state-of-the-art results.

**Objectives:**

1. Develop a computational method for estimating E[LCS] of random binary strings
2. Implement this algorithm efficiently
3. Conduct large-scale numerical experiments to obtain the lower bound
4. Compare the obtained results with the best known bounds from the literature
5. Optimize the implementation for large parameter values (N, shift, K)

---

## Slide 4 — Results

**Current result:**

A new best rigorous lower bound for γ_2 has been obtained:

**γ_2 ≥ 0.7959**

with parameters: N=12, MAX_SHIFT=25, K=10,000

**Experiment parameters:**
- Window size: N = 12 bits
- Maximum shift: SH = 25
- String length: K = 10,000
- Computation time: ~6 hours

| Authors | Year | Lower bound γ₂ |
|---|---|---|
| Chvátal & Sankoff | 1975 | ≥ 0.77 |
| Dančík & Paterson | 1995 | ≥ 0.773911 |
| Lueker | 2003 | ≥ 0.788071 |
| Heineman et al. | 2024 | ≥ 0.792666 |
| **This work** | **2025** | **≥ 0.7959** |

---

## Slide 5 — Solution Prerequisites

**Core idea:**

To obtain a lower bound, we can compute the expected LCS for "chunks" of strings.

Examples obtained by exhaustive search:
- For chunks of length 1, E = 0.5
- For chunks of length 2, E = 0.5625
- For chunks of length 10, E = …

Exhaustive search runs in O(4^n · n^2); for larger chunks we need something better…

**Let us generate the strings on the fly, looking N symbols ahead.**

---

## Slide 6 — Method Description: Windowed Dynamic Programming

**Core idea:**

Instead of analyzing all possible LCS as a whole, we consider local decisions with "windows" of N symbols ahead.

**DP state:**

solve(L, shift, maskA, maskB)

- L — remaining string length
- shift — current shift |i − j| between positions in strings A and B
- maskA, maskB — bitmasks of the next N characters of strings A and B

**Returns:** E[LCS] for the given configuration

**Time / Memory complexity:** O(K · MAX_SH · 4^N)

**DP transition table:**

| Action | Condition | L | Shift |
|---|---|---|---|
| Advance a | shift > 0 | −= 1 | −= 1 |
| Swap a and b | shift == 0 | const | = 1 |
| Advance b | shift < MAX_SH | const | += 1 |
| Also advance a | shift == MAX_SH | −= 1 | const |
| **Take character into LCS** | **maskA[0] == maskB[0]** | **−= 1** | **const** |

---

## Slide 7 — Implementation Optimizations

**Computational optimizations:**

1. **Iterative approach:**
   - Replaced recursion with iteration over L from 1 to K
   - Eliminated function call overhead

2. **Compiler optimizations:**
   - `#pragma GCC optimize("Ofast,no-stack-protector,unroll-loops,fast-math")`
   - `#pragma GCC target("tune=native")`
   - `#pragma GCC target("avx2")`

4. **Parameter selection:**
   - N = 12: window size
   - MAX_SH = 25: maximum shift
   - K = 10,000: string length
   - Runtime: 6 hours
   - Result: 0.7959

**Complexity:**
- Memory: O(MAX_SH · 4^N)
- Time: O(K · MAX_SH · 4^N)

---

## Slide 8 — Future Research Directions

**Possible improvements:**

1. **Algorithmic optimizations:**
   - Pruning useless masks (greedy strategy when visible LCS ≥ 0.5·N)
   - Potential: 100× speedup

2. **Computational improvements:**
   - Scaling to a cluster / supercomputer
   - Adding parallelization

3. **Increasing parameters:**
   - N=12, SH=50, K=1,000,000 → expected bound ~0.8+

4. **Other related bounds:**
   - Upper bound estimation
   - Lower bounds for larger alphabet sizes

---

## Slide 9 — Key References and Publications

**Foundational works:**
- Chvátal, V., Sankoff, D. (1975). Longest common subsequences of two random sequences. J. Appl. Probab. 12(2), 306–315.
- Lueker, G. S. (2009). Improved bounds on the average length of longest common subsequences. J. ACM 56(3), 17:1–17:38.
- Heineman, G. T., Miller, C., Reichman, D., Salls, A., Sarkozy, G., Soiffer, D. (2024). Improved Lower Bounds on the Expected Length of Longest Common Subsequences. ArXiv:2407.10925
- Tiskin, A. (2023). The Chvátal–Sankoff problem as a symbolic dynamics problem. Zap. Nauchn. Sem. POMI 528, 214–237.

**This work:**
Title: Improved Lower Bound for the Chvátal–Sankoff Constant via Windowed Dynamic Programming
Author: Yukachev D. E.
Year: 2025
Result: γ_2 ≥ 0.7959

---

## Slide 10 (Closing)

*(HSE logo — thank you slide)*

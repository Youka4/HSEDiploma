# Presentation Speech (~6–7 minutes)

---

## [Slide 1 — Title] (~10 sec)

Good afternoon. My name is Dmitry Yukachev. Today I will present my thesis: "Improved Lower Bound for the Chvátal–Sankoff Constant via Windowed Dynamic Programming."

---

## [Slide 2 — Introduction] (~1 min 10 sec)

The Longest Common Subsequence, or LCS, is the longest sequence of characters that appears in both strings in the same order, but not necessarily consecutively. For example, the LCS of "ABCD" and "ACBD" is "ABD" — length 3.

Now consider two random binary strings of length n, where each bit is chosen uniformly at random. In 1975, Chvátal and Sankoff showed that the expected LCS length grows linearly: E of L sub n is approximately gamma two times n, where gamma two is the Chvátal–Sankoff constant.

The exact value of gamma two has remained unknown for fifty years. The current best lower bound is 0.7927 by Heineman et al. in 2024, and the upper bound is 0.8263 by Lueker in 2009.

My work focuses on improving the lower bound.

---

## [Slide 3 — Goal and Objectives] (~30 sec)

The goal is to obtain an improved rigorous lower bound for gamma two, surpassing the state of the art.

The objectives are: develop a computational method for estimating the expected LCS; implement it efficiently; run large-scale experiments; compare with known bounds; and optimize for large parameters.

---

## [Slide 4 — Results] (~40 sec)

The main result: I obtained a new best lower bound — gamma two is at least 0.7959.

This was achieved with window size N equals 12, maximum shift 25, string length 10,000, in about 6 hours of computation.

As the table shows, this improves upon every prior bound — from the original 0.77 in 1975, through Lueker's 0.788, to Heineman et al.'s 0.7927. Our 0.7959 is a clear step forward.

---

## [Slide 5 — Solution Prerequisites] (~1 min)

How does the method work?

A standard approach to lower-bounding gamma two is to split the strings into short chunks and compute the expected LCS per chunk. By subadditivity, the average LCS per symbol over short chunks gives a valid lower bound for the full constant.

For short chunks, exhaustive search works: for length 1 the expected match is 0.5; for length 2 it is 0.5625. The longer the chunk, the tighter the bound. But exhaustive search runs in big O of four to the n times n squared — it becomes infeasible already around length 10.

The key insight: instead of enumerating all possible chunk pairs, we can build the strings incrementally, character by character, looking only N symbols ahead at each step. This is the core of the windowed dynamic programming approach.

---

## [Slide 6 — Method Description] (~1 min 10 sec)

Let me describe the DP formally.

The state is solve of L, shift, maskA, maskB. Here L is the remaining string length; shift is the current offset between our reading positions in strings A and B; and maskA, maskB are bitmasks of the next N characters visible in each string.

At each step, we choose the action that maximizes the expected LCS. The transition rules are shown in the table. If the shift is positive, we advance in string A, reducing both L and shift. If the shift is zero, we swap A and B. If the shift is below the maximum, we advance in string B. And crucially — whenever the leading bits of both masks match, we take that character into the LCS, gaining one unit of length.

When a character leaves the window, a new random bit is revealed. We average over both possible values — zero and one — which gives us the expectation. The total complexity is big O of K times max shift times four to the N.

---

## [Slide 7 — Implementation Optimizations] (~45 sec)

With N equals 12, we have about 16 million mask pairs, times 25 shift values, times 10,000 iterations — so efficient implementation is essential.

First, I replaced the recursive formulation with a bottom-up iterative approach, processing L from 1 to K. This eliminates function call overhead entirely.

Second, I applied aggressive compiler optimizations: Ofast, loop unrolling, fast math, and AVX2 vectorization targeting the native architecture.

The final C++ implementation runs in approximately 6 hours on a single machine and produces the bound of 0.7959.

---

## [Slide 8 — Future Directions] (~40 sec)

Several directions for improvement. First, pruning useless mask states — when the visible LCS is already high, many states are redundant. This could give up to a 100x speedup.

Second, parallelization and scaling to a cluster would allow much larger K. With K equals one million we expect the bound to exceed 0.8.

Third, the framework can be adapted for upper bounds and for larger alphabet sizes.

---

## [Slide 9 — References] (~10 sec)

This slide lists the key references the work builds upon.

---

## [Slide 10 — Closing] (~15 sec)

To summarize: using windowed dynamic programming, I obtained a new best lower bound for the Chvátal–Sankoff constant — gamma two is at least 0.7959. The method has clear potential for further improvement.

Thank you. I am happy to take questions.

# Segment Trees with Lazy Propagation

## What This Project Does

This project implements and benchmarks three approaches to a common data 
problem: efficiently computing and updating sums over ranges of data.

- **Naive array** — the brute force baseline
- **Standard Segment Tree** — efficient range queries and point updates
- **Segment Tree with Lazy Propagation** — efficient range queries 
  AND range updates

The benchmark compares all three across dataset sizes ranging from 1,000 
to 100,000,000 elements, making the O(n) vs O(log n) performance gap 
visible and concrete.

## The Data Structure

A Segment Tree is a binary tree built over an array where each node stores 
the precomputed sum of a specific range of elements. This allows range sum 
queries to run in O(log n) time by combining results from a small number of 
relevant nodes rather than scanning the full range.

A standard Segment Tree handles point updates efficiently, but range updates 
remain costly — every affected element needs to be touched. Lazy Propagation 
solves this by deferring updates: instead of immediately applying a change to 
all affected elements, pending updates are stored at higher-level nodes and 
pushed down only when necessary. The result is that both range queries and 
range updates run in O(log n) time.

## Motivation

During my time at Netflix, I witnessed firsthand how quickly data problems 
transform as systems scale. As the company transitioned from a DVD-based 
business to a global streaming platform, data volumes didn't just grow — 
they changed in character entirely. What had once been manageable batch data 
about physical shipments became a continuous, high-velocity stream of every 
play, pause, rewind, and fast-forward across tens of millions of users globally.

The consequences were concrete. Operations that were especially painful 
involved repeated computations over ranges of data: aggregating metrics across 
time windows, applying updates across subsets of records, or recalculating 
rolling statistics as new events arrived. The naive approach — iterating 
through every element in a range — runs in O(n) time and becomes impractical 
at scale.

## How to Compile

From the `src` directory:

```bash
g++ -o benchmark benchmark.cpp naive.cpp segment_tree.cpp lazy_segment_tree.cpp
```

Requires a C++ compiler with C++11 support or later (g++ or clang++).

## How to Run

```bash
./benchmark
```

The benchmark will run automatically across all dataset sizes and print 
results to the console.

## Results

Running the benchmark on a MacBook Pro with Apple Silicon produced the 
following results:

### Range Sum (Naive vs Standard Segment Tree vs Lazy Segment Tree)

| Size | Naive | Segment Tree | Lazy Segment Tree | Winner |
|------|-------|-------------|-------------------|---------|
| 1K | 0.33 ms | 0.001 ms | 0.002 ms | Segment |
| 10K | 2.83 ms | 0.001 ms | 0.002 ms | Segment |
| 100K | 19.03 ms | 0.001 ms | 0.001 ms | Tie |
| 1M | 135.69 ms | 0.001 ms | 0.001 ms | Tie |
| 10M | 1119.08 ms | 0.001 ms | 0.004 ms | Segment |
| 100M | 11510 ms | 0.001 ms | 0.001 ms | Tie |

### Point Update (Naive vs Standard Segment Tree)

| Size | Naive | Segment Tree | Winner |
|------|-------|-------------|---------|
| 1K | 0.002 ms | 0.011 ms | Naive |
| 10K | 0.001 ms | 0.013 ms | Naive |
| 100K | 0.001 ms | 0.017 ms | Naive |
| 1M | 0.001 ms | 0.022 ms | Naive |
| 10M | 0.003 ms | 0.045 ms | Naive |
| 100M | 0.006 ms | 0.077 ms | Naive |

### Range Update (Naive vs Lazy Segment Tree)

| Size | Naive | Lazy Segment Tree | Winner |
|------|-------|-------------------|---------|
| 1K | 0.041 ms | 0.043 ms | Naive |
| 10K | 0.328 ms | 0.049 ms | Lazy |
| 100K | 3.78 ms | 0.093 ms | Lazy |
| 1M | 32.47 ms | 0.144 ms | Lazy |
| 10M | 412.13 ms | 0.234 ms | Lazy |
| 100M | 3647.66 ms | 0.369 ms | Lazy |

## Why This Is Interesting

The results surprised me in some ways. I'd expected a slam dunk for Lazy
all test as the data size grew, but that wasn't necessarily the case. For
Point Updates, Naive was the clear winner, but for mixed workloads that 
also include range updates, the field rapidly narrows until only the Lazy
method is left standing, with outstanding overall performance, even at very
large data sizes.

A few highlights:

- **Range sum** — the Segment Tree and Lazy Segment Tree are effectively 
  instantaneous regardless of dataset size, while naive grows linearly. 
  At 100M elements, naive takes over 11 seconds. The Segment Tree takes 
  less than a millisecond. Lazy only narrowly missed winning.

- **Point update** — an interesting reversal. Naive actually *outperforms* 
  the Segment Tree here because a direct array write (`arr[index] = value`) 
  is a single operation, while the Segment Tree must traverse O(log n) nodes 
  to ripple the change upward.

- **Range update** — Lazy Propagation's moment to shine. At 100M elements, 
  naive takes 3,647ms while Lazy takes 0.37ms — roughly a 10,000x speedup. 
  This is exactly the class of operation that was painful at Netflix scale.

## A Note on Integer Overflow

An early version of this implementation used `int` for range sum return 
values. At small dataset sizes the results looked correct. At 100M elements 
with random values between 1 and 1000, the true sum exceeded `int`'s maximum 
value of ~2.1 billion, producing a result of `-1,494,279,353` — a negative 
number with no compiler warning. The fix was straightforward: switching to 
`long long`, which supports values up to ~9.2 quintillion.

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
| 1K | 0.111 ms | 0.000375 ms | 0.000583 ms | Segment |
| 10K | 1.02 ms | 0.000333 ms | 0.0005 ms | Segment |
| 100K | 10.663 ms | 0.000584 ms | 0.001125 ms | Segment |
| 1M | 115.619 ms | 0.00075 ms | 0.0005 ms | Lazy |
| 10M | 1141.33 ms | 0.000833 ms | 0.003959 ms | Segment |
| 100M | 11651.7 ms | 0.000709 ms | 0.005083 ms | Segment |

### Point Update (Naive vs Standard Segment Tree vs Lazy Segment Tree)

| Size | Naive | Segment Tree | Lazy Segment Tree | Winner |
|------|-------|-------------|-------------------|---------|
| 1K | 0.001542 ms | 0.012334 ms | 0.015917 ms | Naive |
| 10K | 0.001 ms | 0.015208 ms | 0.016042 ms | Naive |
| 100K | 0.001541 ms | 0.03225 ms | 0.027666 ms | Naive |
| 1M | 0.001375 ms | 0.036584 ms | 0.028375 ms | Naive |
| 10M | 0.003708 ms | 0.055167 ms | 0.06025 ms | Naive |
| 100M | 0.004708 ms | 0.070208 ms | 0.074583 ms | Naive |

### Range Update (Naive vs Standard Segment Tree vs Lazy Segment Tree)

| Size | Naive | Segment Tree | Lazy Segment Tree | Winner |
|------|-------|-------------|-------------------|---------|
| 1K | 0.071667 ms | 0.017958 ms | 0.032667 ms | Segment |
| 10K | 0.436583 ms | 0.024958 ms | 0.045333 ms | Segment |
| 100K | 5.4395 ms | 0.040666 ms | 0.092666 ms | Segment |
| 1M | 42.906 ms | 0.03925 ms | 0.144708 ms | Segment |
| 10M | 422.548 ms | 0.081833 ms | 0.23075 ms | Segment |
| 100M | 4068.01 ms | 0.108667 ms | 0.3705 ms | Segment |

### Narrow Range Update - 1000 element range (Naive vs Standard Segment Tree vs Lazy Segment Tree)

| Size | Naive | Segment Tree | Lazy Segment Tree | Winner |
|------|-------|-------------|-------------------|---------|
| 1K | skipped | skipped | skipped | N/A |
| 10K | 0.17325 ms | 0.025875 ms | 0.043875 ms | Segment |
| 100K | 0.117708 ms | 0.029333 ms | 0.060125 ms | Segment |
| 1M | 0.17525 ms | 0.02975 ms | 0.083458 ms | Segment |
| 10M | 0.142667 ms | 0.058458 ms | 0.130583 ms | Segment |
| 100M | 0.203208 ms | 0.09225 ms | 0.214041 ms | Segment |

## Why This Is Interesting

The results surprised me. I expected Lazy Propagation to dominate across 
all tests as data size grew, but that wasn't the case.

Here's what the data actually shows, and why:

- **Point update** — Naive wins decisively. A direct array write 
  (`arr[index] = value`) is a single operation, while both tree 
  implementations must traverse O(log n) nodes to ripple the change upward.

- **Range sum** — Both tree implementations are effectively instantaneous 
  regardless of dataset size, while Naive grows linearly. At 100M elements, 
  Naive takes over 11 seconds while both trees take under a millisecond. 
  Standard Segment Tree narrowly edges out Lazy in most runs.

- **Range update (wide and narrow)** — Standard Segment Tree surprisingly 
  outperforms Lazy in both tests. The reason: this benchmark measures update 
  time in isolation, with no subsequent queries. Lazy Propagation's deferred 
  update strategy only pays off when updates are followed by queries,
  allowing pending work to be skipped entirely if a node is never visited. 
  When every update is immediately "due", the overhead of maintaining the 
  lazy array adds cost without saving any work.

The key takeaway is that **there is no universally best data structure** — 
the right choice depends entirely on your operational workload:

- If your system is dominated by **range queries**, either tree implementation 
  works. Both are dramatically faster than Naive.
- If your system performs frequent **point updates**, Naive is surprisingly 
  competitive.
- If your system mixes **range updates with range queries** at **large 
  scale**, Lazy Propagation should shine, but this benchmark doesn't test this 
  scenario directly. A mixed-workload benchmark would be needed to confirm 
  this theoretical advantage empirically.

Regardless, understanding your operational or usage profile before choosing a 
data structure is the real lesson here.

## Limitations and Future Work

- **Mixed workload testing** — the current benchmark measures each operation 
  in isolation. A more realistic benchmark would mix range updates and 
  range queries to better reveal Lazy Propagation's theoretical advantage.
- **Integer overflow** — an early version used `int` for range sum return 
  values. At 100M elements with random values between 1 and 1000, the true 
  sum exceeded `int`'s maximum of ~2.1 billion, silently producing 
  `-1,494,279,353` with no compiler warning. Fixed by switching to `long long`.

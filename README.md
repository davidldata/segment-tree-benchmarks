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
remain costly since every affected element needs to be touched. Lazy Propagation 
solves this by deferring updates: instead of immediately applying a change to 
all affected elements, pending updates are stored at higher-level nodes and 
pushed down only when necessary. The result is that both range queries and 
range updates run in O(log n) time.

## Motivation

During my time at Netflix, I witnessed firsthand how quickly data problems 
transform as systems scale. As the company transitioned from a DVD-based 
business to a global streaming platform, data volumes grew exponentially. 
What had once been manageable batch data about physical shipments became a 
continuous, high-velocity stream of every play, pause, rewind, and 
fast-forward across tens of millions of users and hundreds of millions of
devices globally.

The consequences of this data explosion were significant and immediate. 
Processes that previously completed in reasonable times measured in several hours
now needed to be measured in half day increments. Operations that were especially 
painful involved repeated computations over ranges of data, such as aggregating 
metrics across time windows, applying updates across subsets of records, or 
recalculating rolling statistics as new events arrived. The naive approach — iterating 
through every element in a range — runs in O(n) time and becomes impractical 
at scale.

So, for this project, I decided to test and benchmark the performance of 
several data structures against some of the scenarios and conditions we faced
at Netflix.

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
| 1K | 0.117 ms | 0.000333 ms | 0.000792 ms | Segment |
| 10K | 1.096 ms | 0.000375 ms | 0.000584 ms | Segment |
| 100K | 14.348 ms | 0.000917 ms | 0.002708 ms | Segment |
| 1M | 109.306 ms | 0.000375 ms | 0.000875 ms | Segment |
| 10M | 1171.54 ms | 0.000875 ms | 0.001291 ms | Segment |
| 100M | 11446.7 ms | 0.000792 ms | 0.005083 ms | Segment |

### Point Update (Naive vs Standard Segment Tree vs Lazy Segment Tree)

| Size | Naive | Segment Tree | Lazy Segment Tree | Winner |
|------|-------|-------------|-------------------|---------|
| 1K | 0.005958 ms | 0.016875 ms | 0.018875 ms | Naive |
| 10K | 0.009417 ms | 0.023167 ms | 0.02475 ms | Naive |
| 100K | 0.005834 ms | 0.036084 ms | 0.029416 ms | Naive |
| 1M | 0.006833 ms | 0.046667 ms | 0.070583 ms | Naive |
| 10M | 0.010875 ms | 0.060458 ms | 0.104625 ms | Naive |
| 100M | 0.015208 ms | 0.073334 ms | 0.145333 ms | Naive |

### Range Update (Naive vs Standard Segment Tree vs Lazy Segment Tree)

| Size | Naive | Segment Tree | Lazy Segment Tree | Winner |
|------|-------|-------------|-------------------|---------|
| 1K | 0.068083 ms | 0.277791 ms | 0.060292 ms | Lazy |
| 10K | 0.487208 ms | 3.02483 ms | 0.062459 ms | Lazy |
| 100K | 4.44562 ms | 26.4102 ms | 0.101541 ms | Lazy |
| 1M | 49.4151 ms | 268.392 ms | 0.184208 ms | Lazy |
| 10M | 499.767 ms | 2626.15 ms | 0.286333 ms | Lazy |
| 100M | 4995.55 ms | 29957.4 ms | 0.408042 ms | Lazy |

### Narrow Range Update - 1000 element range (Naive vs Standard Segment Tree vs Lazy Segment Tree)

| Size | Naive | Segment Tree | Lazy Segment Tree | Winner |
|------|-------|-------------|-------------------|---------|
| 1K | skipped | skipped | skipped | N/A |
| 10K | 0.173292 ms | 0.842084 ms | 0.056375 ms | Lazy |
| 100K | 0.163541 ms | 0.831625 ms | 0.085083 ms | Lazy |
| 1M | 0.192625 ms | 0.766458 ms | 0.104708 ms | Lazy |
| 10M | 0.183375 ms | 0.948584 ms | 0.15575 ms | Lazy |
| 100M | 0.223333 ms | 0.942208 ms | 0.311875 ms | Lazy |

## Why This Is Interesting

The benchmark results surprised me. I expected Lazy Propagation to dominate across 
all tests as data size grew, but that wasn't at all the case.

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
  lazy array adds cost without saving any work. So, in this scenario, Lazy
  is essentially Segment + Lazy-array-maintenance, which is why Lazy didn't
  win at any of the range_update tests.

The key takeaway is that **there is no universally best data structure**, 
the right choice depends entirely on your operational workload:

- If your system is dominated by **range queries**, either tree implementation 
  works, and both are dramatically faster than Naive.
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

#include "naive.h"
#include "segment_tree.h"
#include "lazy_segment_tree.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <functional>
#include <random>

using namespace std;

// Timing helper function 
double time_operation(function<void()> operation) {
    auto start = chrono::high_resolution_clock::now();
    operation();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed = end - start;
    return elapsed.count();
}

// Benchmark parameters
const int repetitions = 100;
const int size_1k = 1000;
const int size_10k = 10000;
const int size_100k = 100000;
const int size_1m = 1000000;
const int size_10m = 10000000;
const int size_100m = 100000000;

// Generate array of given size with random integers between 1 and 1000
vector<int> generate_random_array(int size) {
    vector<int> arr(size);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 1000);
    for (int i = 0; i < size; ++i) {
        arr[i] = dis(gen);
    }
    return arr;
}

// Helper function for benchmarking range_sum queries
void benchmark_range_sum(int n){
    // Generate a random array of size n
    vector<int> arr = generate_random_array(n);

    // Build data structures
    vector<long long> seg_tree(4 * n, 0);
    segtree::build(arr, seg_tree, 0, 0, n - 1);
    
    vector<long long> lazy_tree(4 * n, 0);
    vector<long long> lazy(4 * n, 0);
    lazytree::build(arr, lazy_tree, 0, 0, n - 1);

    // Time Naive Range Sum
    volatile long long naive_checksum = 0;
    
    double naive_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            naive_checksum += naive::range_sum(arr, 0, n - 1);
        }
    });

    // Time Segment Tree Range Sum
    volatile long long seg_checksum = 0;
    
    double seg_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            seg_checksum += segtree::range_sum(seg_tree, 0, 0, n - 1, 0, n - 1);
            }
        });

    // Time Lazy Segment Tree Range Sum
    volatile long long lazy_checksum = 0;
    
    double lazy_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            lazy_checksum += lazytree::range_sum(lazy_tree, lazy, 0, 0, n - 1, 0, n - 1);
        }
    });

    // Print results
    cout << "\n** Range Sum Benchmark **" << endl;
    cout << "Size: " << n << endl;
    cout << " - Naive: " << naive_time << " ms" << endl;
    cout << " - Segment Tree: " << seg_tree_time << " ms" << endl;
    cout << " - Lazy Segment Tree: " << lazy_tree_time << " ms" << endl;
    //cout << " - Naive checksum: " << naive_checksum << endl;
    //cout << " - Segment Tree checksum: " << seg_checksum << endl;
    //cout << " - Lazy Segment Tree checksum: " << lazy_checksum << endl;
}

// Helper function for point update benchmarks
void benchmark_point_update(int n) {

    struct PointOp {
        int index;
        int value;
    };

    // Generate a random array of size n
    vector<int> arr = generate_random_array(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> index_dist(0, n - 1);
    uniform_int_distribution<> value_dist(1, 1000);

    // Pre-generate point updates for all structures to ensure fairness
    vector<PointOp> ops;
    for (int i = 0; i < repetitions; ++i) {
        ops.push_back({index_dist(gen), value_dist(gen)});
    }

    // Build data structures
    vector<long long> seg_tree(4 * n, 0);
    segtree::build(arr, seg_tree, 0, 0, n - 1);
    vector<long long> lazy_tree(4 * n, 0);
    vector<long long> lazy(4 * n, 0);
    lazytree::build(arr, lazy_tree, 0, 0, n - 1);


    // Time Naive Point Update
    double naive_time = time_operation([&]() {
        for (const auto& op : ops) {
            naive::point_update(arr, op.index, op.value);
        }
    });
    volatile long long naive_checksum = naive::range_sum(arr, 0, n - 1);

    // Time Segment Tree Point Update
    double seg_tree_time = time_operation([&]() {
        for (const auto& op : ops) {
            segtree::point_update(seg_tree, 0, 0, n - 1, op.index, op.value);
        }
    });
    volatile long long seg_checksum = segtree::range_sum(seg_tree, 0, 0, n - 1, 0, n - 1);

    // Time Lazy Segment Tree Point Update
    double lazy_tree_time = time_operation([&]() {
        for (const auto& op : ops) {
            lazytree::point_update(lazy_tree, lazy, 0, 0, n - 1, op.index, op.value);
        }
    });
    volatile long long lazy_checksum = lazytree::range_sum(lazy_tree, lazy, 0, 0, n - 1, 0, n - 1);

    // Print results
    cout << "\n** Point Update Benchmark **" << endl;
    cout << "Size: " << n << endl;
    cout << " - Naive: " << naive_time << " ms" << endl;
    cout << " - Segment Tree: " << seg_tree_time << " ms" << endl;
    cout << " - Lazy Segment Tree: " << lazy_tree_time << " ms" << endl;
    //cout << " - Naive checksum: " << naive_checksum << endl;
    //cout << " - Segment Tree checksum: " << seg_checksum << endl;
    //cout << " - Lazy Segment Tree checksum: " << lazy_checksum << endl;
}

// Helper function for range update benchmarks
void benchmark_range_update(int n) {

    struct RangeOp {
        int left;
        int right;
        int value;
    };

    // Generate a random array of size n
    vector<int> arr = generate_random_array(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> index_dist(0, n - 1);
    uniform_int_distribution<> value_dist(1, 1000);

    vector<RangeOp> ops;
    for (int i = 0; i < repetitions; ++i) {
        int left = index_dist(gen);
        int right = index_dist(gen);
        if (left > right) swap(left, right);
        int value = value_dist(gen);
        ops.push_back({left, right, value});
    }

    // Build data structures
    vector<long long> lazy_tree(4 * n, 0);
    vector<long long> lazy(4 * n, 0);
    lazytree::build(arr, lazy_tree, 0, 0, n - 1);
    vector<long long> seg_tree(4 * n, 0);
    segtree::build(arr, seg_tree, 0, 0, n - 1);

    // Time Naive Range Update
    double naive_time = time_operation([&]() {
        for (const auto& op : ops) {
            naive::range_update(arr, op.left, op.right, op.value);
        }
    });
    volatile long long naive_checksum = naive::range_sum(arr, 0, n - 1);

    // Time Lazy Segment Tree Range Update
    double lazy_tree_time = time_operation([&]() {
        for (const auto& op : ops) {
            lazytree::range_update(lazy_tree, lazy, 0, 0, n - 1, op.left, op.right, op.value);
        }
    });
    volatile long long lazy_checksum = lazytree::range_sum(lazy_tree, lazy, 0, 0, n - 1, 0, n - 1);

    // Time Segment Tree Range Update
    double seg_tree_time = time_operation([&]() {
        for (const auto& op : ops) {
            segtree::range_update(seg_tree, 0, 0, n - 1, op.left, op.right, op.value);
        }
    });
    volatile long long seg_checksum = segtree::range_sum(seg_tree, 0, 0, n - 1, 0, n - 1);

    // Print results
    cout << "\n** Range Update Benchmark **" << endl;
    cout << "Size: " << n << endl;
    cout << " - Naive: " << naive_time << " ms" << endl;
    cout << " - Segment Tree: " << seg_tree_time << " ms" << endl;
    cout << " - Lazy Segment Tree: " << lazy_tree_time << " ms" << endl;
    //cout << " - Naive checksum: " << naive_checksum << endl;
    //cout << " - Segment Tree checksum: " << seg_checksum << endl;
    //cout << " - Lazy Segment Tree checksum: " << lazy_checksum << endl;

}

int main() {

    cout << "====================================" << endl;
    cout << "          Benchmark Results      " << endl;
    cout << "====================================" << endl;

    cout << "\n-- Point Update --" << endl;
    benchmark_point_update(size_1k);
    benchmark_point_update(size_10k);
    benchmark_point_update(size_100k);
    benchmark_point_update(size_1m);
    benchmark_point_update(size_10m);
    benchmark_point_update(size_100m);

    cout << "\n-- Range Sum --" << endl;
    benchmark_range_sum(size_1k);
    benchmark_range_sum(size_10k);
    benchmark_range_sum(size_100k);
    benchmark_range_sum(size_1m);
    benchmark_range_sum(size_10m);
    benchmark_range_sum(size_100m);

    cout << "\n-- Range Update --" << endl;
    benchmark_range_update(size_1k);
    benchmark_range_update(size_10k);
    benchmark_range_update(size_100k);
    benchmark_range_update(size_1m);
    benchmark_range_update(size_10m);
    benchmark_range_update(size_100m);

    return 0;
}
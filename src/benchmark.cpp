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
    vector<int> seg_tree(4 * n, 0);
    segtree::build(arr, seg_tree, 0, 0, n - 1);
    
    vector<int> lazy_tree(4 * n, 0);
    vector<int> lazy(4 * n, 0);
    lazytree::build(arr, lazy_tree, 0, 0, n - 1);

    // Time Naive Range Sum
    double naive_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            naive::range_sum(arr, 0, n - 1);
        }
    });

    // Time Segment Tree Range Sum
    double seg_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            segtree::range_sum(seg_tree, 0, 0, n - 1, 0, n - 1);
        }
    });

    // Time Lazy Segment Tree Range Sum
    double lazy_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            lazytree::range_sum(lazy_tree, lazy, 0, 0, n - 1, 0, n - 1);
        }
    });

    // Print results
    cout << "\n** Range Sum Benchmark **" << endl;
    cout << "Size: " << n << endl;
    cout << " - Naive: " << naive_time << " ms" << endl;
    cout << " - Segment Tree: " << seg_tree_time << " ms" << endl;
    cout << " - Lazy Segment Tree: " << lazy_tree_time << " ms" << endl;
}

// Helper function for point update benchmarks
void benchmark_point_update(int n) {
    // Generate a random array of size n
    vector<int> arr = generate_random_array(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> index_dist(0, n - 1);
    uniform_int_distribution<> value_dist(1, 1000);

    // Build data structures
    vector<int> seg_tree(4 * n, 0);
    segtree::build(arr, seg_tree, 0, 0, n - 1);
    vector<int> lazy_tree(4 * n, 0);
    vector<int> lazy(4 * n, 0);
    lazytree::build(arr, lazy_tree, 0, 0, n - 1);


    // Time Naive Point Update
    double naive_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int index = index_dist(gen);
            int new_value = value_dist(gen);
            naive::point_update(arr, index, new_value);
        }
    });

    // Time Segment Tree Point Update
    double seg_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int index = index_dist(gen);
            int new_value = value_dist(gen);
            segtree::point_update(seg_tree, 0, 0, n - 1, index, new_value);
        }
    });

    // Time Lazy Segment Tree Point Update
    double lazy_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int index = index_dist(gen);
            int new_value = value_dist(gen);
            lazytree::point_update(lazy_tree, lazy, 0, 0, n - 1, index, new_value);
        }
    });

    // Print results
    cout << "\n** Point Update Benchmark **" << endl;
    cout << "Size: " << n << endl;
    cout << " - Naive: " << naive_time << " ms" << endl;
    cout << " - Segment Tree: " << seg_tree_time << " ms" << endl;
    cout << " - Lazy Segment Tree: " << lazy_tree_time << " ms" << endl;
}

// Helper function for range update benchmarks
void benchmark_range_update(int n) {
    // Generate a random array of size n
    vector<int> arr = generate_random_array(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> index_dist(0, n - 1);
    uniform_int_distribution<> value_dist(1, 1000);

    // Build data structures
    vector<int> lazy_tree(4 * n, 0);
    vector<int> lazy(4 * n, 0);
    lazytree::build(arr, lazy_tree, 0, 0, n - 1);
    vector<int> seg_tree(4 * n, 0);
    segtree::build(arr, seg_tree, 0, 0, n - 1);

    // Time Naive Range Update
    double naive_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int left = index_dist(gen);
            int right = index_dist(gen);
            if (left > right) swap(left, right);
            int new_value = value_dist(gen);
            naive::range_update(arr, left, right, new_value);
        }
    });

    // Time Lazy Segment Tree Range Update
    double lazy_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int left = index_dist(gen);
            int right = index_dist(gen);
            if (left > right) swap(left, right);
            int new_value = value_dist(gen);
            lazytree::range_update(lazy_tree, lazy, 0, 0, n - 1, left, right, new_value);
        }
    });

    // Time Segment Tree Range Update
    double seg_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int left = index_dist(gen);
            int right = index_dist(gen);
            if (left > right) swap(left, right);
            int new_value = value_dist(gen);
            segtree::range_update(seg_tree, 0, 0, n - 1, left, right, new_value);
        }
    });


    // Print results
    cout << "\n** Range Update Benchmark **" << endl;
    cout << "Size: " << n << endl;
    cout << " - Naive: " << naive_time << " ms" << endl;
    cout << " - Segment Tree: " << seg_tree_time << " ms" << endl;
    cout << " - Lazy Segment Tree: " << lazy_tree_time << " ms" << endl;

}

// Helper function for narrow range update benchmarks
void benchmark_narrow_range_update(int n) {
    if (n <= 1000) {
        cout << "\n** Narrow Range Update Benchmark (1000 element range) **" << endl;
        cout << "Size: " << n << " -- skipped (dataset too small for 1000-element range)" << endl;
        return;
    }

    // Generate a random array of size n
    vector<int> arr = generate_random_array(n);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> value_dist(1, 1000);
    uniform_int_distribution<> narrow_dist(0, n - 1001);

    // Build data structures
    vector<int> lazy_tree(4 * n, 0);
    vector<int> lazy(4 * n, 0);
    lazytree::build(arr, lazy_tree, 0, 0, n - 1);
    vector<int> seg_tree(4 * n, 0);
    segtree::build(arr, seg_tree, 0, 0, n - 1);

    // Time Naive Range Update
    double naive_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int left = narrow_dist(gen);
            int right = left + 999; // always exactly 1000 elements
            int new_value = value_dist(gen);
            naive::range_update(arr, left, right, new_value);
        }
    });

    // Time Lazy Segment Tree Range Update
    double lazy_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int left = narrow_dist(gen);
            int right = left + 999; // always exactly 1000 elements
            int new_value = value_dist(gen);
            lazytree::range_update(lazy_tree, lazy, 0, 0, n - 1, left, right, new_value);
        }
    });

    // Time Segment Tree Range Update
    double seg_tree_time = time_operation([&]() {
        for (int i = 0; i < repetitions; ++i) {
            int left = narrow_dist(gen);
            int right = left + 999; // always exactly 1000 elements
            int new_value = value_dist(gen);
            segtree::range_update(seg_tree, 0, 0, n - 1, left, right, new_value);
        }
    });

    // Print results
    cout << "\n** Narrow Range Update Benchmark (1000 elements) **" << endl;
    cout << "Size: " << n << endl;
    cout << " - Naive: " << naive_time << " ms" << endl;
    cout << " - Segment Tree: " << seg_tree_time << " ms" << endl;
    cout << " - Lazy Segment Tree: " << lazy_tree_time << " ms" << endl;

}

int main() {

    // Quick overflow check
    cout << "Long long Sanity Check" << endl;
    int n = 100000000;
    vector<int> arr = generate_random_array(n);
    long long expected = 0;
    for (int i = 0; i < n; i++) expected += arr[i];
    cout << "Expected (long long): " << expected << endl;
    cout << "Got (int):            " << naive::range_sum(arr, 0, n - 1) << endl;

    cout << "====================================" << endl;
    cout << "       Segment Tree Benchmarks      " << endl;
    cout << "====================================" << endl;

    cout << "\n-- Range Sum --" << endl;
    benchmark_range_sum(size_1k);
    benchmark_range_sum(size_10k);
    benchmark_range_sum(size_100k);
    benchmark_range_sum(size_1m);
    benchmark_range_sum(size_10m);
    benchmark_range_sum(size_100m);


    cout << "\n-- Point Update --" << endl;
    benchmark_point_update(size_1k);
    benchmark_point_update(size_10k);
    benchmark_point_update(size_100k);
    benchmark_point_update(size_1m);
    benchmark_point_update(size_10m);
    benchmark_point_update(size_100m);


    cout << "\n-- Range Update --" << endl;
    benchmark_range_update(size_1k);
    benchmark_range_update(size_10k);
    benchmark_range_update(size_100k);
    benchmark_range_update(size_1m);
    benchmark_range_update(size_10m);
    benchmark_range_update(size_100m);

    cout << "\n-- Narrow Range Update (1000 elements) --" << endl;
    benchmark_narrow_range_update(size_1k);
    benchmark_narrow_range_update(size_10k);
    benchmark_narrow_range_update(size_100k);
    benchmark_narrow_range_update(size_1m);
    benchmark_narrow_range_update(size_10m);
    benchmark_narrow_range_update(size_100m);

    return 0;
}
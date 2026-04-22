#include "naive.h"
#include <iostream>
#include <vector>

using namespace std;

namespace naive {
    // Naive implementation of segment tree operations for benchmarking purposes.
    // Tests three functions: point update, range sum query, and range update.
    
    void point_update(vector<int>& arr, int index, int value) {
        arr[index] = value;
    }

    long long range_sum(vector<int>& arr, int left, int right) {
        long long sum = 0;
        for (int i = left; i <= right; i++) {
            sum += arr[i];
        }
        return sum;
    }

    void range_update(vector<int>& arr, int left, int right, int value) {
        for (int i = left; i <= right; i++) {
            arr[i] += value;
        }
    }
}

// Renamed main function that was used to test the naive implementation
void naive_test() {
    using namespace naive;
    
    // Create a testbed with 1000 elements initialized to 1    
    int n = 1000;
    vector<int> testbed(n, 1);

    // Test each of the functions and print the results.
    cout << endl;
    cout << "************************************" << endl << endl;

    cout << "**Test Range Sum**" << endl;
    cout << "Range sum [0, 999] should be 1000: " << range_sum(testbed, 0, 999) << endl;
    cout << endl;

    cout << "**Test Point Update**" << endl;
    cout << "Original value at index 500: " << testbed[500] << endl; // Should print 1
    cout << "**Update Index 500 to 42**" << endl;
    point_update(testbed, 500, 42);
    cout << "New value at index 500: " << testbed[500] << endl; // Should print 42
    cout << "New range sum [499, 501] should be 1+42+1 = 44: " << range_sum(testbed, 499, 501) << endl;
    cout << endl;

    cout << "**Test Range Update add 5 to first 100 elements**" << endl;

    range_update(testbed, 0, 99, 5);
    cout << "After range update [0,99] +5, sum [0,99] should be 600: " << range_sum(testbed, 0, 99) << endl;
    cout << endl;
    cout << "************************************" << endl << endl;

    return ;
}
#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;

void radixSort(vector<int>& arr) {
    int maxDigit = 0;
    for (int num : arr) {
        int digitCount = 0;
        while (num > 0) {
            num /= 10;
            digitCount++;
        }
        maxDigit = max(maxDigit, digitCount);
    }
    vector<int> count(10);
    for (int digit = 0, exp = 1; digit < maxDigit; digit++, exp *= 10) {
        fill(count.begin(), count.end(), 0);
        for (int num : arr) {
            int digitVal = (num / exp) % 10;
            count[digitVal]++;
        }
        for (int i = 1; i < count.size(); i++) {
            count[i] += count[i - 1];
        }
        vector<int> sortedArr(arr.size());
        for (int i = arr.size() - 1; i >= 0; i--) {
            int digitVal = (arr[i] / exp) % 10;
            int index = count[digitVal] - 1;
            sortedArr[index] = arr[i];
            count[digitVal]--;
        }
        arr = sortedArr;
    }
}

void mergeSortHelper(vector<int>& arr, vector<int>& temp, int start, int end) {
    if (start >= end) {
        return;
    }
    int mid = (start + end) / 2;
    mergeSortHelper(arr, temp, start, mid);
    mergeSortHelper(arr, temp, mid + 1, end);
    int left = start;
    int right = mid + 1;
    for (int i = start; i <= end; i++) {
        if (left > mid) {
            temp[i] = arr[right++];
        } else if (right > end) {
            temp[i] = arr[left++];
        } else if (arr[left] <= arr[right]) {
            temp[i] = arr[left++];
        } else {
            temp[i] = arr[right++];
        }
    }
    for (int i = start; i <= end; i++) {
        arr[i] = temp[i];
    }
}

void mergeSort(vector<int>& arr) {
    vector<int> temp(arr.size());
    mergeSortHelper(arr, temp, 0, arr.size() - 1);
}

void shellSort(vector<int>& arr) {
    int gap = arr.size() / 2;
    while (gap > 0) {
        for (int i = gap; i < arr.size(); i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
        gap /= 2;
    }
}

void heapify(vector<int>& arr, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }
    if (largest != i) {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}
void heapSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
    for (int i = n - 1; i > 0; i--) {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

void bucketSort(vector<int>& arr) {
    int numBuckets = 10;
    vector<vector<int>> buckets(numBuckets);
    int maxVal = *max_element(arr.begin(), arr.end());
    for (int num : arr) {
        int bucketIndex = (num * numBuckets) / (maxVal + 1);
        buckets[bucketIndex].push_back(num);
    }
    for (vector<int>& bucket : buckets) {
        sort(bucket.begin(), bucket.end());
    }
    int index = 0;
    for (vector<int>& bucket : buckets) {
        for (int num : bucket) {
            arr[index++] = num;
        }
    }
}

int main() {
    ifstream inputFile("teste.in");
    int numArrays;
    inputFile >> numArrays;
    vector<vector<int>> arrays(numArrays);
    for (int i = 0; i < numArrays; i++) {
        int arrSize;
        inputFile >> arrSize;
        vector<int> arr(arrSize);
        for (int j = 0; j < arrSize; j++) {
            inputFile >> arr[j];
        }
        arrays[i] = arr;
    }
    inputFile.close();

    vector<string> methodNames = {"RadixSort", "MergeSort", "ShellSort", "HeapSort", "BucketSort"};
    vector<void (*)(vector<int>&)> methods = {radixSort, mergeSort, shellSort, heapSort, bucketSort};
    int numMethods = methodNames.size();
    vector<long long> executionTimes(numMethods);

    for (int i = 0; i < numArrays; i++) {
        cout << "Array " << i + 1 << ":" << endl;
        for (int j = 0; j < numMethods; j++) {
            vector<int> arr = arrays[i];
            auto start = chrono::steady_clock::now();
            methods[j](arr);
            auto end = chrono::steady_clock::now();
            executionTimes[j] += chrono::duration_cast<chrono::microseconds>(end - start).count();
            if (!is_sorted(arr.begin(), arr.end())) {
                cout << methodNames[j] << " failed to sort array." << endl;
                break;
            }
            cout << methodNames[j] << " execution time: " << chrono::duration_cast<chrono::microseconds>(end - start).count() << " microseconds" << endl;
        }
        cout << endl;
    }

    cout << "Total execution times:" << endl;
    for (int i = 0; i < numMethods; i++) {
        cout << methodNames[i] << " execution time: " << executionTimes[i] << " microseconds" << endl;
    }

    return 0;
}

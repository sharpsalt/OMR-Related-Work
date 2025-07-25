// #include <iostream>
// #include <fstream>
// #include <vector>
// #include <algorithm>
// #include <chrono>

// double duration_ns;

// // Read flattened array from a text file
// std::vector<int> readFlattenedArray(const std::string& filePath) {
//     std::vector<int> array;
//     std::ifstream inFile(filePath);
//     if (!inFile.is_open()) {
//         std::cerr << "Error: Could not open '" << filePath << "'" << std::endl;
//         return array;
//     }

//     int val;
//     try {
//         array.reserve(1000000); // Pre-allocate memory
//         while (inFile >> val) {
//             array.push_back(val);
//         }
//     } catch (const std::bad_alloc& e) {
//         std::cerr << "Memory error reading '" << filePath << "': " << e.what() << std::endl;
//         array.clear();
//     }
//     inFile.close();
//     return array;
// }

// // Memory-efficient LCS computation
// int computeLCS(const std::vector<int>& arr1, const std::vector<int>& arr2) {
//     const int m = arr1.size();
//     const int n = arr2.size();

//     auto start = std::chrono::high_resolution_clock::now();

//     std::vector<std::vector<int>> dp(2, std::vector<int>(n + 1, 0));

//     for (int i = 1; i <= m; i++) {
//         for (int j = 1; j <= n; j++) {
//             if (arr1[i - 1] == arr2[j - 1]) {
//                 dp[i % 2][j] = dp[(i - 1) % 2][j - 1] + 1;
//             } else {
//                 dp[i % 2][j] = std::max(dp[(i - 1) % 2][j], dp[i % 2][j - 1]);
//             }
//         }
//     }

//     auto end = std::chrono::high_resolution_clock::now();
//     duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

//     return dp[m % 2][n];
// }

// int main() {
//     try {
//         // Read arrays from files
//         std::vector<int> arr1 = readFlattenedArray("Sample.txt");
//         std::vector<int> arr2 = readFlattenedArray("Test.txt");

//         if (arr1.empty() || arr2.empty()) {
//             std::cerr << "Error: One or both input arrays are empty." << std::endl;
//             return 1;
//         }

//         std::cout << "Array sizes: " << arr1.size() << " and " << arr2.size() << " elements\n";

//         // Compute LCS length
//         int lcsLength = computeLCS(arr1, arr2);
//         int totalLength = arr1.size() + arr2.size();

//         // More balanced similarity measure
//         float similarity = (2.0f * lcsLength / totalLength) * 100.0f;

//         // Output
//         std::cout << "LCS Length: " << lcsLength << std::endl;
//         std::cout << "Max Possible Combined Length: " << totalLength << std::endl;
//         std::cout << "Similarity: " << similarity << "%" << std::endl;
//         std::cout << "Execution Time: " << duration_ns << " ns (" 
//                   << duration_ns / 1e9 << " ms)" << std::endl;

//     } catch (const std::exception& e) {
//         std::cerr << "Exception: " << e.what() << std::endl;
//         return 1;
//     }

//     return 0;
// }
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <string>

double duration_ns;

// Read flattened array from a text file
std::vector<int> readFlattenedArray(const std::string& filePath) {
    std::vector<int> array;
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open '" << filePath << "'" << std::endl;
        return array;
    }

    int val;
    try {
        array.reserve(1000000);
        while (inFile >> val) {
            array.push_back(val);
        }
    } catch (const std::bad_alloc& e) {
        std::cerr << "Memory error reading '" << filePath << "': " << e.what() << std::endl;
        array.clear();
    }
    inFile.close();
    return array;
}

// Memory-efficient LCS computation
int computeLCS(const std::vector<int>& arr1, const std::vector<int>& arr2) {
    const int m = arr1.size();
    const int n = arr2.size();

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::vector<int>> dp(2, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (arr1[i - 1] == arr2[j - 1]) {
                dp[i % 2][j] = dp[(i - 1) % 2][j - 1] + 1;
            } else {
                dp[i % 2][j] = std::max(dp[(i - 1) % 2][j], dp[i % 2][j - 1]);
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    return dp[m % 2][n];
}

int main() {
    std::ofstream resultFile("results.txt");
    if (!resultFile.is_open()) {
        std::cerr << "Error: Could not open results.txt for writing." << std::endl;
        return 1;
    }

    resultFile << std::fixed << std::setprecision(2);
    resultFile << "Index\tSize1\tSize2\tLCS\tSimilarity(%)\tTime(ns)\tTime(ms)\n";

    for (int i = 1; i <= 15; ++i) {
        std::string file1 = "sample" + std::to_string(i) + ".txt";
        std::string file2 = "test" + std::to_string(i) + ".txt";
        std::cout<<i<<std::endl;
        std::vector<int> arr1 = readFlattenedArray(file1);
        std::vector<int> arr2 = readFlattenedArray(file2);

        if (arr1.empty() || arr2.empty()) {
            resultFile << i << "\tError reading files\n";
            continue;
        }

        int lcsLength = computeLCS(arr1, arr2);
        int totalLength = arr1.size() + arr2.size();
        float similarity = (2.0f * lcsLength / totalLength) * 100.0f;

        resultFile << i << "\t"
                   << arr1.size() << "\t"
                   << arr2.size() << "\t"
                   << lcsLength << "\t"
                   << similarity << "\t"
                   << duration_ns << "\t"
                   << duration_ns / 1e6 << "\n";

        std::cout << "Processed pair " << i << ": " << file1 << " & " << file2 << std::endl;
    }

    resultFile.close();
    std::cout << "Results written to results.txt\n";

    return 0;
}

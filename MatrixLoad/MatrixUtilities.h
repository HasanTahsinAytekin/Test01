//
// Created by Hasan Aytekin on 16.01.2024.
//

#ifndef MATRIXLOAD_MATRIXUTILITIES_H
#define MATRIXLOAD_MATRIXUTILITIES_H

#include <vector>
#include <cmath>
#include <queue>
#include <algorithm>
#include <utility>
#include <iostream>

namespace Matrix {

    class MatrixUtilities {
// Function to calculate Euclidean distance between two points
        double euclidean_distance(const std::vector<double>& p1, const std::vector<double>& p2) {
            double sum = 0.0;
            for (int i = 0; i < p1.size(); i++) {
                sum += pow(p1[i] - p2[i], 2);
            }
            return sqrt(sum);
        }

// Function to find the k-nearest neighbors of a target point in a matrix
        std::vector<std::vector<double>> find_k_nearest_neighbors(const std::vector<std::vector<double>>& matrix,
                                                        const std::vector<double>& target, int k) {
            std::vector<std::vector<double>> neighbors;  // To store the k nearest neighbors
            std::priority_queue<std::pair<double, std::vector<double>>> max_heap;  // For efficient distance tracking

            // Calculate distances for all points and store in max heap
            for (const auto& point : matrix) {
                double distance = euclidean_distance(point, target);
                max_heap.push({-distance, point});  // Use negative distance for descending order
                if (max_heap.size() > k) {  // Maintain only k nearest points
                    max_heap.pop();
                }
            }

            // Retrieve the k nearest neighbors from the heap
            while (!max_heap.empty()) {
                neighbors.push_back(max_heap.top().second);
                max_heap.pop();
            }

            reverse(neighbors.begin(), neighbors.end());  // Rearrange in ascending distance order

            return neighbors;
        }

        int type1_main() {
            // Example usage
            std::vector<std::vector<double>> matrix = {{1, 2}, {3, 4}, {5, 6}};
            std::vector<double> target = {2, 3};
            int k = 2;

            std::vector<std::vector<double>> neighbors = find_k_nearest_neighbors(matrix, target, k);

            // Print the k nearest neighbors
            for (const auto& neighbor : neighbors) {
                for (double value : neighbor) {
                    std::cout << value << " ";
                }
                std::cout << std::endl;
            }

            return 0;
        }

    // 22222222222222222222222222222222222222222222222222222

        // A function to calculate the Euclidean distance between two vectors
        double euclidean_distance2(const std::vector<double>& v1, const std::vector<double>& v2) {
            double sum = 0;
            for (int i = 0; i < v1.size(); i++) {
                sum += (v1[i] - v2[i]) * (v1[i] - v2[i]);
            }
            return sqrt(sum);
        }

// A function to find the k-nearest neighbors of a given vector in a matrix
        std::vector<int> k_nearest_neighbors2(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vec, int k) {
            std::vector<std::pair<double, int>> distances; // A vector of pairs of distance and index
            for (int i = 0; i < matrix.size(); i++) {
                double dist = euclidean_distance(matrix[i], vec); // Calculate the distance between the matrix row and the vector
                distances.push_back(std::make_pair(dist, i)); // Add the pair to the vector
            }
            sort(distances.begin(), distances.end()); // Sort the vector by the distance
            std::vector<int> neighbors; // A vector to store the indices of the k-nearest neighbors
            for (int i = 0; i < k; i++) {
                neighbors.push_back(distances[i].second); // Add the index of the i-th nearest neighbor
            }
            return neighbors;
        }

// A function to print a vector
        void print_vector(const std::vector<int>& vec) {
            std::cout << "[";
            for (int i = 0; i < vec.size(); i++) {
                std::cout << vec[i];
                if (i < vec.size() - 1) {
                    std::cout << ", ";
                }
            }
            std::cout << "]" << std::endl;
        }

// A main function to test the code
        int main2() {
            // A sample matrix
            std::vector<std::vector<double>> matrix = {
                    {1, 2, 3},
                    {4, 5, 6},
                    {7, 8, 9},
                    {10, 11, 12}
            };
            // A sample vector
            std::vector<double> vec = {2, 3, 4};
            // A sample value of k
            int k = 2;
            // Find the k-nearest neighbors of the vector in the matrix
            std::vector<int> neighbors = k_nearest_neighbors2(matrix, vec, k);
            // Print the result
            std::cout << "The " << k << "-nearest neighbors of the vector are: " << std::endl;
            print_vector(neighbors);
            return 0;
        }

    };

} // Matrix

#endif //MATRIXLOAD_MATRIXUTILITIES_H

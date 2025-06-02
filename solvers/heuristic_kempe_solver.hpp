#pragma once

#include "base_solver.hpp"
#include <unordered_set>
#include <algorithm>
#include <iostream>

template <int N>
class HeuristicKempeSolver : public BaseSolver<N> {
    using typename BaseSolver<N>::Board;
    using typename BaseSolver<N>::Matrix;
    using BaseSolver<N>::SIZE;

private:
    const Matrix* adjMatrix;
    static constexpr int K = N;  // K is equal to N (4 or 9)
    std::array<int, SIZE> best_values;

    int find_vertex_with_degree_less_than_k(const std::array<int, SIZE>& values) {
        for (int i = 0; i < SIZE; ++i) {
            if (values[i] == -1) {  // Only consider uncolored vertices
                int degree = 0;
                for (int j = 0; j < SIZE; ++j) {
                    if ((*adjMatrix)[i][j]) {  // Count all adjacent vertices
                        degree++;
                    }
                }
                if (degree < K) {
                    return i;
                }
            }
        }
        return -1;
    }

    std::unordered_set<int> get_used_colors(const std::array<int, SIZE>& values, int vertex) {
        std::unordered_set<int> used_colors;
        for (int i = 0; i < SIZE; ++i) {
            if ((*adjMatrix)[vertex][i] && values[i] != -1) {
                used_colors.insert(values[i]);
            }
        }
        return used_colors;
    }

    bool is_valid_color(const std::array<int, SIZE>& values, int vertex, int color) {
        for (int i = 0; i < SIZE; ++i) {
            if ((*adjMatrix)[vertex][i] && values[i] == color) {
                return false;
            }
        }
        return true;
    }

    bool recursive_solve(std::array<int, SIZE>& values, int depth = 0) {
        // Safety check to prevent infinite recursion
        if (depth > SIZE) {
            std::cerr << "Heuristic Kempe solver failed: Maximum recursion depth exceeded\n";
            return false;
        }

        // Find a vertex with degree less than K
        int vertex = find_vertex_with_degree_less_than_k(values);
        
        if (vertex == -1) {
            // No vertex with degree < K found
            // Find any uncolored vertex
            for (int i = 0; i < SIZE; ++i) {
                if (values[i] == -1) {
                    vertex = i;
                    break;
                }
            }
            
            if (vertex == -1) {
                return true;  // All vertices are colored
            }
        }

        // Get colors used by adjacent vertices
        auto used_colors = get_used_colors(values, vertex);

        // If less than K colors are used by adjacent vertices
        if (used_colors.size() < K) {
            // Try each unused color
            bool found_color = false;
            for (int color = 0; color < K; ++color) {
                if (used_colors.find(color) == used_colors.end()) {
                    this->steps++;  // Count each color attempt
                    values[vertex] = color;
                    if (recursive_solve(values, depth + 1)) {
                        found_color = true;
                        break;
                    }
                    values[vertex] = -1;
                }
            }
            if (!found_color) {
                return false;
            }
        } else {
            // If K or more colors are used, try each color
            bool found_color = false;
            for (int color = 0; color < K; ++color) {
                if (is_valid_color(values, vertex, color)) {
                    this->steps++;  // Count each color attempt
                    values[vertex] = color;
                    if (recursive_solve(values, depth + 1)) {
                        found_color = true;
                        break;
                    }
                    values[vertex] = -1;
                }
            }
            if (!found_color) {
                return false;
            }
        }

        // Save the current state as the best attempt
        best_values = values;
        return true;
    }

public:
    void solve(Board& board, const Matrix& adj_matrix) override {
        adjMatrix = &adj_matrix;
        std::array<int, SIZE> values;
        std::fill(values.begin(), values.end(), -1);
        std::fill(best_values.begin(), best_values.end(), -1);

        // Initialize with pre-colored values
        for (int i = 0; i < SIZE; ++i) {
            if (board[i / N][i % N].value != -1) {
                values[i] = board[i / N][i % N].value;
                best_values[i] = values[i];
            }
        }

        bool success = recursive_solve(values);
        if (!success) {
            std::cerr << "Heuristic Kempe solver failed to find a solution.\n";
            // Print where it got stuck
            for (int i = 0; i < SIZE; ++i) {
                if (best_values[i] == -1) {
                    std::cerr << "Failed at position " << i << " (" << i / N << "," << i % N << ")\n";
                }
            }
        }

        // Always apply the best attempt, even if we failed
        for (int i = 0; i < SIZE; ++i) {
            board[i / N][i % N].value = best_values[i];
        }
    }
}; 
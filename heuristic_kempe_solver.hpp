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
            for (int color = 0; color < K; ++color) {
                if (used_colors.find(color) == used_colors.end()) {
                    values[vertex] = color;
                    if (recursive_solve(values, depth + 1)) {
                        return true;
                    }
                    values[vertex] = -1;
                }
            }
        } else {
            // If K or more colors are used, try each color
            for (int color = 0; color < K; ++color) {
                if (is_valid_color(values, vertex, color)) {
                    values[vertex] = color;
                    if (recursive_solve(values, depth + 1)) {
                        return true;
                    }
                    values[vertex] = -1;
                }
            }
        }

        return false;
    }

public:
    void solve(Board& board, const Matrix& adj_matrix) override {
        adjMatrix = &adj_matrix;
        std::array<int, SIZE> values;
        std::fill(values.begin(), values.end(), -1);

        // Initialize with pre-colored values
        for (int i = 0; i < SIZE; ++i) {
            if (board[i / N][i % N].value != -1) {
                values[i] = board[i / N][i % N].value;
            }
        }

        if (recursive_solve(values)) {
            for (int i = 0; i < SIZE; ++i) {
                board[i / N][i % N].value = values[i];
            }
        } else {
            std::cerr << "No solution exists for this puzzle.\n";
        }
    }
}; 
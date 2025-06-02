#pragma once

#include "base_solver.hpp"
#include <algorithm>
#include <iostream>

template <int N>
class BacktrackingSolver : public BaseSolver<N> {
    using typename BaseSolver<N>::Board;
    using typename BaseSolver<N>::Matrix;
    using BaseSolver<N>::SIZE;

private:
    const Matrix* adjMatrix;
    std::array<int, SIZE> best_values;

    bool is_safe(const std::array<int, SIZE>& values, int pos, int color) {
        for (int i = 0; i < SIZE; ++i) {
            if ((*adjMatrix)[pos][i] && values[i] == color) {
                return false;
            }
        }
        return true;
    }

    int find_mrv_position(const std::array<int, SIZE>& values) {
        int min_remaining = N + 1;
        int chosen_pos = -1;

        for (int pos = 0; pos < SIZE; ++pos) {
            if (values[pos] == -1) {
                int remaining = count_remaining_values(values, pos);
                if (remaining < min_remaining) {
                    min_remaining = remaining;
                    chosen_pos = pos;
                }
            }
        }
        return chosen_pos;
    }

    int count_remaining_values(const std::array<int, SIZE>& values, int pos) {
        std::array<bool, N> available;
        std::fill(available.begin(), available.end(), true);

        for (int i = 0; i < SIZE; ++i) {
            if ((*adjMatrix)[pos][i] && values[i] != -1 && values[i] < N) {
                available[values[i]] = false;
            }
        }

        return std::count(available.begin(), available.end(), true);
    }

    int count_constraints(const std::array<int, SIZE>& values, int pos, int color) {
        int count = 0;
        for (int i = 0; i < SIZE; ++i) {
            if ((*adjMatrix)[pos][i] && values[i] == -1) {
                if (!is_safe(values, i, color)) {
                    count++;
                }
            }
        }
        return count;
    }

    bool backtrack_solve(std::array<int, SIZE>& values, int depth) {
        if (depth >= SIZE) {
            return true;
        }

        int pos = find_mrv_position(values);
        if (pos == -1) {
            return true;
        }

        std::array<std::pair<int, int>, N> color_constraints;
        for (int color = 0; color < N; ++color) {
            color_constraints[color] = {count_constraints(values, pos, color), color};
        }
        std::sort(color_constraints.begin(), color_constraints.end());

        for (const auto& [constraints, color] : color_constraints) {
            this->steps++;  // Count each color attempt
            if (is_safe(values, pos, color)) {
                values[pos] = color;
                if (backtrack_solve(values, depth + 1)) {
                    return true;
                }
                values[pos] = -1;
            }
        }

        // Save the current state as the best attempt
        best_values = values;
        return false;
    }

public:
    void solve(Board& board, const Matrix& adj_matrix) override {
        adjMatrix = &adj_matrix;
        std::array<int, SIZE> values;
        std::fill(values.begin(), values.end(), -1);
        std::fill(best_values.begin(), best_values.end(), -1);

        for (int i = 0; i < SIZE; ++i) {
            if (board[i / N][i % N].value != -1) {
                values[i] = board[i / N][i % N].value;
                best_values[i] = values[i];
            }
        }

        if (backtrack_solve(values, 0)) {
            // If we found a solution, use the solved values
            for (int i = 0; i < SIZE; ++i) {
                board[i / N][i % N].value = values[i];
            }
        } else {
            std::cerr << "No solution exists for this puzzle.\n";
            // If we failed, use the best attempt
            for (int i = 0; i < SIZE; ++i) {
                board[i / N][i % N].value = best_values[i];
            }
        }
    }
}; 
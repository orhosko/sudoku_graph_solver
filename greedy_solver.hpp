#pragma once

#include "base_solver.hpp"
#include <algorithm>
#include <iostream>

template <int N>
class GreedySolver : public BaseSolver<N> {
    using typename BaseSolver<N>::Board;
    using typename BaseSolver<N>::Matrix;
    using BaseSolver<N>::SIZE;

public:
    void solve(Board& board, const Matrix& adjMatrix) override {
        std::array<int, SIZE> values;
        std::fill(values.begin(), values.end(), -1);

        for (int i = 0; i < SIZE; ++i) {
            if (board[i / N][i % N].value != -1) {
                values[i] = board[i / N][i % N].value;
            }
        }

        for (int i = 0; i < SIZE; ++i) {
            std::array<bool, N> available;
            std::fill(available.begin(), available.end(), true);

            for (int j = 0; j < SIZE; ++j) {
                if (adjMatrix[i][j] == 1 && values[j] != -1) {
                    available[values[j]] = false;
                }
            }

            int color = -1;
            for (int c = 0; c < N; ++c) {
                if (available[c]) {
                    color = c;
                    break;
                }
            }

            if (color == -1) {
                std::cerr << "Greedy failed. No available color for square " << i << "\n";
                return;
            }

            values[i] = color;
        }

        for (int i = 0; i < SIZE; ++i) {
            board[i / N][i % N].value = values[i];
        }
    }
}; 
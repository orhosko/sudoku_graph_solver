#pragma once

#include "base_solver.hpp"
#include <unordered_set>
#include <algorithm>
#include <iostream>

template <int N>
class DSaturSolver : public BaseSolver<N> {
    using typename BaseSolver<N>::Board;
    using typename BaseSolver<N>::Matrix;
    using BaseSolver<N>::SIZE;

public:
    void solve(Board& board, const Matrix& adjMatrix) override {
        std::array<int, SIZE> colors;
        colors.fill(-1);

        for (int i = 0; i < SIZE; ++i) {
            if (board[i / N][i % N].value != -1) {
                colors[i] = board[i / N][i % N].value;
            }
        }

        std::array<std::unordered_set<int>, SIZE> neighbor_colors;
        std::array<int, SIZE> degrees{};
        std::array<bool, SIZE> colored{};

        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (adjMatrix[i][j])
                    degrees[i]++;
            }
        }

        for (int step = 0; step < SIZE; ++step) {
            int max_sat = -1, max_deg = -1, selected = -1;

            for (int i = 0; i < SIZE; ++i) {
                if (colored[i])
                    continue;

                int sat = static_cast<int>(neighbor_colors[i].size());

                if (sat > max_sat || (sat == max_sat && degrees[i] > max_deg)) {
                    max_sat = sat;
                    max_deg = degrees[i];
                    selected = i;
                }
            }

            std::array<bool, N> available{};
            available.fill(true);

            for (int j = 0; j < SIZE; ++j) {
                if (adjMatrix[selected][j] && colors[j] != -1) {
                    available[colors[j]] = false;
                }
            }

            int chosen_color = -1;
            for (int c = 0; c < N; ++c) {
                if (available[c]) {
                    chosen_color = c;
                    break;
                }
            }

            if (chosen_color == -1) {
                std::cerr << "DSatur failed: no available color for square " << selected << '\n';
                return;
            }

            colors[selected] = chosen_color;
            colored[selected] = true;

            for (int j = 0; j < SIZE; ++j) {
                if (adjMatrix[selected][j] && !colored[j]) {
                    neighbor_colors[j].insert(chosen_color);
                }
            }
        }

        for (int i = 0; i < SIZE; ++i) {
            board[i / N][i % N].value = colors[i];
        }
    }
}; 
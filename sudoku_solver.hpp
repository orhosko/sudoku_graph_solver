#pragma once

#include "base_solver.hpp"
#include "greedy_solver.hpp"
#include "dsatur_solver.hpp"
#include "backtracking_solver.hpp"
#include "heuristic_kempe_solver.hpp"
#include <memory>
#include <cmath>

// A constexpr function to check if a number is a perfect square
constexpr bool is_perfect_square(int n) {
    if (n <= 0)
        return false;
    int root = static_cast<int>(std::sqrt(n));
    return root * root == n;
}

// Define a concept for perfect squares
template <int N>
concept PerfectSquare = is_perfect_square(N);

// Enum to select solver type
enum class SolverType {
    Greedy,
    DSatur,
    Backtracking,
    HeuristicKempe
};

// Modified SudokuSolver to use templates for solver selection
template <int N, SolverType Type>
    requires PerfectSquare<N>
class SudokuSolver {
public:
    static constexpr int SIZE = N * N;
    using Board = std::array<std::array<Square, N>, N>;
    using Matrix = std::array<std::array<int, SIZE>, SIZE>;

    Board board;
    Matrix adjMatrix{};

    SudokuSolver(const Board &initial_board) : board(initial_board) {}

    void create_row_deps() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < N; ++j) {
                int row_start = (i / N) * N;
                int col = row_start + j;
                if (col != i) {
                    adjMatrix[i][col] += 1;
                }
            }
        }
    }

    void create_col_deps() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < N; ++j) {
                int col_start = i % N;
                int row = j * N + col_start;
                if (row != i) {
                    adjMatrix[i][row] += 1;
                }
            }
        }
    }

    void create_block_deps() {
        int block_size = static_cast<int>(std::sqrt(N));
        for (int i = 0; i < SIZE; ++i) {
            int row = i / N;
            int col = i % N;
            int block_row = row / block_size;
            int block_col = col / block_size;
            for (int dr = 0; dr < block_size; ++dr) {
                for (int dc = 0; dc < block_size; ++dc) {
                    int r = block_row * block_size + dr;
                    int c = block_col * block_size + dc;
                    int j = r * N + c;
                    if (j != i) {
                        adjMatrix[i][j] += 1;
                    }
                }
            }
        }
    }

    void normalize_adj_matrix() {
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                adjMatrix[i][j] = std::min(adjMatrix[i][j], 1);
            }
        }
    }

    void solve() {
        create_row_deps();
        create_col_deps();
        create_block_deps();
        normalize_adj_matrix();

        std::unique_ptr<BaseSolver<N>> solver;
        if constexpr (Type == SolverType::Greedy) {
            solver = std::make_unique<GreedySolver<N>>();
        } else if constexpr (Type == SolverType::DSatur) {
            solver = std::make_unique<DSaturSolver<N>>();
        } else if constexpr (Type == SolverType::Backtracking) {
            solver = std::make_unique<BacktrackingSolver<N>>();
        } else {
            solver = std::make_unique<HeuristicKempeSolver<N>>();
        }

        solver->solve(board, adjMatrix);
    }

    void print_board() const {
        for (const auto &row : board) {
            for (const auto &sq : row) {
                if (sq.value == 0) {
                    printf("%2d ", N);
                } else {
                    printf("%2d ", sq.value);
                }
            }
            std::cout << "\n";
        }
    }

    void print_adj_matrix() const {
        for (const auto &row : adjMatrix) {
            for (const auto &val : row) {
                printf("%d ", val);
            }
            std::cout << "\n";
        }
    }
}; 
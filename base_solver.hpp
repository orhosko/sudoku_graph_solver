#pragma once

#include <array>
#include <memory>

struct Square {
    int index;
    int value;
};

// Base solver interface
template <int N>
class BaseSolver {
public:
    using Board = std::array<std::array<Square, N>, N>;
    using Matrix = std::array<std::array<int, N * N>, N * N>;
    static constexpr int SIZE = N * N;

    virtual ~BaseSolver() = default;
    virtual void solve(Board& board, const Matrix& adjMatrix) = 0;
}; 
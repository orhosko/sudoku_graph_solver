#pragma once

#include <array>
#include <cstddef>
#include "../common/types.hpp"

template <int N>
class BaseSolver {
public:
    static constexpr int SIZE = N * N;
    using Board = std::array<std::array<Square, N>, N>;
    using Matrix = std::array<std::array<int, SIZE>, SIZE>;

    BaseSolver() : steps(0) {}
    virtual void solve(Board& board, const Matrix& adjMatrix) = 0;
    virtual ~BaseSolver() = default;

    std::size_t get_steps() const { return steps; }

protected:
    std::size_t steps;
}; 
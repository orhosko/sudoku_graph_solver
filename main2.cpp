#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <unordered_set>

#include <concepts>

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

struct Square {
    int index;
    int value;
};

template <int N>
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

    void greedy_coloring() {
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
                std::cerr << "Greedy failed. No available color for square "
                          << i << "\n";
                std::cout << "===========================\n";

                for (int j = 0; j < i; ++j) {
                    board[j / N][j % N].value = values[j];
                }

                for (int a : values) {
                    std::cout << a << " ";
                }
                std::cout << "\n";

                for (int a : available) {
                    std::cout << a << " ";
                }
                std::cout << "\n";

                return;
            }

            values[i] = color;
        }

        for (int i = 0; i < SIZE; ++i) {
            board[i / N][i % N].value = values[i];
        }
    }

    void dsatur_coloring() {
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

        // TODO: just don't normalize
        // Precompute degrees
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (adjMatrix[i][j])
                    degrees[i]++;
            }
        }

        for (int step = 0; step < SIZE; ++step) {
            // Select vertex with highest saturation (break ties with degree)
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

            // Find smallest available color
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
                std::cerr << "DSatur failed: no available color for square "
                          << selected << '\n';

                // Apply to board
                for (int i = 0; i < SIZE; ++i) {
                    board[i / N][i % N].value = colors[i];
                }
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

        // Apply to board
        for (int i = 0; i < SIZE; ++i) {
            board[i / N][i % N].value = colors[i];
        }
    }

    void solve() {
        create_row_deps();
        create_col_deps();
        create_block_deps();
        normalize_adj_matrix();
        // print_adj_matrix();
        // greedy_coloring();
        dsatur_coloring();
    }

    void print_board() const {
        for (const auto &row : board) {
            for (const auto &sq : row) {
                printf("%2d ", sq.value);
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

int main() {
    std::array<std::array<Square, 4>, 4> initial_board{
        std::array<Square, 4>{Square{0, 0}, Square{1, 1}, Square{2, -1},
                              Square{3, 3}},
        std::array<Square, 4>{Square{4, -1}, Square{5, 2}, Square{6, -1},
                              Square{7, 0}},
        std::array<Square, 4>{Square{8, 2}, Square{9, -1}, Square{10, 0},
                              Square{11, 1}},
        std::array<Square, 4>{Square{12, 1}, Square{13, 0}, Square{14, -1},
                              Square{15, -1}}};

    SudokuSolver<4> solver(initial_board);
    solver.print_board();
    std::cout << "=========================\n";
    std::cout << "Solving Sudoku...\n";
    std::cout << "=========================\n";
    solver.solve();
    solver.print_board();

    std::array<std::array<Square, 9>, 9> initial_board_9{
        std::array<Square, 9>{Square{0, 5}, Square{1, 3}, Square{2, -1},
                              Square{3, -1}, Square{4, 7}, Square{5, -1},
                              Square{6, -1}, Square{7, -1}, Square{8, -1}},
        std::array<Square, 9>{Square{9, 6}, Square{10, -1}, Square{11, -1},
                              Square{12, 1}, Square{13, 0}, Square{14, 5},
                              Square{15, -1}, Square{16, -1}, Square{17, -1}},
        std::array<Square, 9>{Square{18, -1}, Square{19, 0}, Square{20, 8},
                              Square{21, -1}, Square{22, -1}, Square{23, -1},
                              Square{24, -1}, Square{25, 6}, Square{26, -1}},
        std::array<Square, 9>{Square{27, 8}, Square{28, -1}, Square{29, -1},
                              Square{30, -1}, Square{31, 6}, Square{32, -1},
                              Square{33, -1}, Square{34, -1}, Square{35, 3}},
        std::array<Square, 9>{Square{36, 4}, Square{37, -1}, Square{38, -1},
                              Square{39, 8}, Square{40, -1}, Square{41, 3},
                              Square{42, -1}, Square{43, -1}, Square{44, 1}},
        std::array<Square, 9>{Square{45, 7}, Square{46, -1}, Square{47, -1},
                              Square{48, -1}, Square{49, 2}, Square{50, -1},
                              Square{51, -1}, Square{52, -1}, Square{53, 6}},
        std::array<Square, 9>{Square{54, -1}, Square{55, 6}, Square{56, -1},
                              Square{57, -1}, Square{58, -1}, Square{59, -1},
                              Square{60, 2}, Square{61, 8}, Square{62, -1}},
        std::array<Square, 9>{Square{63, -1}, Square{64, -1}, Square{65, -1},
                              Square{66, 4}, Square{67, 1}, Square{68, 0},
                              Square{69, -1}, Square{70, -1}, Square{71, 5}},
        std::array<Square, 9>{Square{72, -1}, Square{73, -1}, Square{74, -1},
                              Square{75, -1}, Square{76, 8}, Square{77, -1},
                              Square{78, -1}, Square{79, 7}, Square{80, 0}},
    };

    std::array<std::array<Square, 9>, 9> initial_board_9_2{
        std::array<Square, 9>{Square{0, 0}, Square{1, 1}, Square{2, 2},
                              Square{3, 3}, Square{4, 4}, Square{5, 5},
                              Square{6, 6}, Square{7, 7}, Square{8, -1}},
        std::array<Square, 9>{Square{9, 3}, Square{10, 4}, Square{11, 5},
                              Square{12, 6}, Square{13, 7}, Square{14, 8},
                              Square{15, 0}, Square{16, 1}, Square{17, -1}},
        std::array<Square, 9>{Square{18, 6}, Square{19, 7}, Square{20, 8},
                              Square{21, 0}, Square{22, 1}, Square{23, 2},
                              Square{24, 3}, Square{25, 4}, Square{26, -1}},
        std::array<Square, 9>{Square{27, 1}, Square{28, 2}, Square{29, 3},
                              Square{30, 4}, Square{31, 5}, Square{32, 6},
                              Square{33, 7}, Square{34, 8}, Square{35, -1}},
        std::array<Square, 9>{Square{36, 4}, Square{37, 5}, Square{38, 6},
                              Square{39, 7}, Square{40, 8}, Square{41, 0},
                              Square{42, 1}, Square{43, 2}, Square{44, -1}},
        std::array<Square, 9>{Square{45, 7}, Square{46, 8}, Square{47, 0},
                              Square{48, 1}, Square{49, 2}, Square{50, 3},
                              Square{51, 4}, Square{52, 5}, Square{53, -1}},
        std::array<Square, 9>{Square{54, 2}, Square{55, 3}, Square{56, 4},
                              Square{57, 5}, Square{58, 6}, Square{59, 7},
                              Square{60, 8}, Square{61, 0}, Square{62, -1}},
        std::array<Square, 9>{Square{63, 5}, Square{64, 6}, Square{65, 7},
                              Square{66, 8}, Square{67, 0}, Square{68, 1},
                              Square{69, 2}, Square{70, 3}, Square{71, -1}},
        std::array<Square, 9>{Square{72, 8}, Square{73, 0}, Square{74, 1},
                              Square{75, 2}, Square{76, 3}, Square{77, 4},
                              Square{78, 5}, Square{79, 6}, Square{80, -1}},
    };

    std::cout << "=========================\n";
    std::cout << "=========================\n";
    std::cout << "=========================\n";

    SudokuSolver<9> solver_9(initial_board_9_2);
    solver_9.print_board();
    std::cout << "=========================\n";
    std::cout << "Solving Sudoku...\n";
    std::cout << "=========================\n";
    solver_9.solve();
    solver_9.print_board();
}

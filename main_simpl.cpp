#include <algorithm>
#include <array>
#include <iostream>

struct Square {
    int index;
    int value;  // color
};

std::array<std::array<Square, 4>, 4> board{
    std::array<Square, 4>{Square{0, 0}, Square{1, 1}, Square{2, -1}, Square{3, 3}},
    std::array<Square, 4>{Square{4, -1}, Square{5, 2}, Square{6, -1}, Square{7, 0}},
    std::array<Square, 4>{Square{8, 2}, Square{9, -1}, Square{10, 0}, Square{11, 1}},
    std::array<Square, 4>{Square{12, 3}, Square{13, 0}, Square{14, -1}, Square{15, -1}}};

std::array<std::array<int, 16>, 16> adjMatrix{};

void create_row_deps(std::array<std::array<int, 16>, 16> &adjMartix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i % 4 != j) {
                adjMatrix[i][(i / 4) * 4 + j] += 1;
            }
        }
    }
}

void create_col_deps(std::array<std::array<int, 16>, 16> adjMartix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i / 4 != j) {
                adjMatrix[i][(i % 4) + j * 4] += 1;
            }
        }
    }
}

void create_block_deps(std::array<std::array<int, 16>, 16> adjMartix) {
    for (int i = 0; i < 16; ++i) {
        int x = i % 4;
        int y = i / 4;
        int block_index = (y / 2) * 2 + (x / 2);

        int block_min = (block_index % 2) * 2 + (block_index / 2) * 8;

        adjMatrix[i][block_min] += 1;
        adjMatrix[i][block_min + 1] += 1;
        adjMatrix[i][block_min + 4] += 1;
        adjMatrix[i][block_min + 5] += 1;

        adjMatrix[i][i] -= 1;
    }
}

void normalize_adj_matrix(std::array<std::array<int, 16>, 16> &adjMatrix) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            adjMatrix[i][j] = std::min(adjMatrix[i][j], 1);
        }
    }
}

void greedy_coloring(std::array<std::array<Square, 4>, 4> &board,
                     std::array<std::array<int, 16>, 16> &adjMatrix) {
    std::array<int, 16> values;
    std::ranges::fill(values, -1);

    for (int i = 0; i < 16; ++i) {
        std::array<bool, 4> available;
        std::ranges::fill(available, true);

        for (int j = 0; j < 16; ++j) {
            if (adjMatrix[i][j] == 1 && values[j] != -1) {
                available[values[j]] = false;
            }
        }

        int cr;
        for (cr = 0; cr < 4; ++cr) {
            if (available[cr]) {
                break;
            }
        }

        if (cr == 4) {
            std::cout << "Greedy failed. No available color for square " << i << std::endl;
            return;
        }

        values[i] = cr;
    }

    for (int i = 0; i < 16; ++i) {
        board[i / 4][i % 4].value = values[i];
    }
}

int main() {
    create_row_deps(adjMatrix);
    create_col_deps(adjMatrix);
    create_block_deps(adjMatrix);

    normalize_adj_matrix(adjMatrix);

    std::cout << "===========================" << std::endl;
    std::cout << "Adjacency Matrix:" << std::endl;

    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            std::cout << adjMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "===========================" << std::endl;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%2d ", board[i][j].value);
        }
        std::cout << std::endl;
    }

    std::cout << "===========================" << std::endl;

    // solve by coloring
    greedy_coloring(board, adjMatrix);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            printf("%2d ", board[i][j].value);
        }
        std::cout << std::endl;
    }

    return 0;
}

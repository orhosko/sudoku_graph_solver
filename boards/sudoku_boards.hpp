#pragma once

#include <array>
#include "common/types.hpp"

namespace SudokuBoards {

// 4x4 Sudoku board
constexpr std::array<std::array<Square, 4>, 4> BOARD_4x4{
    std::array<Square, 4>{Square{0, 0}, Square{1, 1}, Square{2, -1},
                          Square{3, 3}},
    std::array<Square, 4>{Square{4, -1}, Square{5, 2}, Square{6, -1},
                          Square{7, 0}},
    std::array<Square, 4>{Square{8, 2}, Square{9, -1}, Square{10, 0},
                          Square{11, 1}},
    std::array<Square, 4>{Square{12, 1}, Square{13, 0}, Square{14, -1},
                          Square{15, -1}}
};

// 9x9 Sudoku board
constexpr std::array<std::array<Square, 9>, 9> BOARD_9x9{
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
                          Square{78, -1}, Square{79, 7}, Square{80, 0}}
};

// 9x9 Extreme Sudoku board
constexpr std::array<std::array<Square, 9>, 9> BOARD_9x9_EXTREME{
    std::array<Square, 9>{Square{0, -1}, Square{1, -1}, Square{2, -1},
                          Square{3, -1}, Square{4, -1}, Square{5, -1},
                          Square{6, -1}, Square{7, 6}, Square{8, 3}},
    std::array<Square, 9>{Square{9, 5}, Square{10, -1}, Square{11, -1},
                          Square{12, 4}, Square{13, -1}, Square{14, 8},
                          Square{15, -1}, Square{16, -1}, Square{17, -1}},
    std::array<Square, 9>{Square{18, 1}, Square{19, -1}, Square{20, -1},
                          Square{21, -1}, Square{22, 3}, Square{23, -1},
                          Square{24, -1}, Square{25, 4}, Square{26, -1}},
    std::array<Square, 9>{Square{27, 2}, Square{28, -1}, Square{29, -1},
                          Square{30, 8}, Square{31, -1}, Square{32, -1},
                          Square{33, -1}, Square{34, 9}, Square{35, -1}},
    std::array<Square, 9>{Square{36, -1}, Square{37, -1}, Square{38, -1},
                          Square{39, -1}, Square{40, -1}, Square{41, -1},
                          Square{42, -1}, Square{43, -1}, Square{44, -1}},
    std::array<Square, 9>{Square{45, -1}, Square{46, -1}, Square{47, 1},
                          Square{48, -1}, Square{49, -1}, Square{50, 7},
                          Square{51, 4}, Square{52, -1}, Square{53, -1}},
    std::array<Square, 9>{Square{54, -1}, Square{55, -1}, Square{56, 6},
                          Square{57, 3}, Square{58, -1}, Square{59, -1},
                          Square{60, -1}, Square{61, -1}, Square{62, -1}},
    std::array<Square, 9>{Square{63, -1}, Square{64, -1}, Square{65, 9},
                          Square{66, 2}, Square{67, -1}, Square{68, -1},
                          Square{69, -1}, Square{70, 1}, Square{71, -1}},
    std::array<Square, 9>{Square{72, -1}, Square{73, -1}, Square{74, 7},
                          Square{75, -1}, Square{76, 8}, Square{77, -1},
                          Square{78, -1}, Square{79, 6}, Square{80, 9}}
};

} // namespace SudokuBoards 
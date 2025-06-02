#include <array>
#include <iostream>
#include <string>

#include "boards/sudoku_boards.hpp"
#include "common/sudoku_solver.hpp"

void print_usage(const char* program_name) {
  std::cout << "Usage: " << program_name << " <solver_type> [board_name]\n"
            << "Solver types:\n"
            << "  greedy - Greedy solver\n"
            << "  dsatur - DSatur solver\n"
            << "  backtrack - Backtracking solver\n"
            << "  kempe - Heuristic Kempe solver\n"
            << "Board names:\n"
            << "  4x4 - 4x4 board (default)\n"
            << "  9x9 - 9x9 board\n"
            << "  9x9_extreme - Extreme 9x9 board\n";
}

SolverType parse_solver_type(const std::string& type) {
  if (type == "greedy") return SolverType::Greedy;
  if (type == "dsatur") return SolverType::DSatur;
  if (type == "backtrack") return SolverType::Backtracking;
  if (type == "kempe") return SolverType::HeuristicKempe;
  throw std::invalid_argument("Invalid solver type");
}

std::string solver_type_to_string(SolverType type) {
  switch (type) {
    case SolverType::Greedy:
      return "Greedy";
    case SolverType::DSatur:
      return "DSatur";
    case SolverType::Backtracking:
      return "Backtracking";
    case SolverType::HeuristicKempe:
      return "Heuristic Kempe";
    default:
      return "Unknown";
  }
}

template <int N, SolverType Type>
void execute_solver(const std::array<std::array<Square, N>, N>& board) {
  SudokuSolver<N, Type> solver(board);
  solver.print_board();
  std::cout << "=========================\n";
  std::cout << "Solving Sudoku...\n";
  std::cout << "=========================\n";
  solver.solve();
  solver.print_board();
  std::cout << "Steps taken: " << solver.get_steps() << "\n";
}

template <int N>
void solve_board_impl(const std::array<std::array<Square, N>, N>& board,
                      SolverType type) {
  switch (type) {
    case SolverType::Greedy:
      execute_solver<N, SolverType::Greedy>(board);
      break;
    case SolverType::DSatur:
      execute_solver<N, SolverType::DSatur>(board);
      break;
    case SolverType::Backtracking:
      execute_solver<N, SolverType::Backtracking>(board);
      break;
    case SolverType::HeuristicKempe:
      execute_solver<N, SolverType::HeuristicKempe>(board);
      break;
  }
}

int main(int argc, char* argv[]) {
  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  try {
    SolverType solver_type = parse_solver_type(argv[1]);
    std::string board_name = (argc > 2) ? argv[2] : "4x4";

    std::cout << "Using " << solver_type_to_string(solver_type) << " solver on "
              << board_name << " board:\n";

    if (board_name == "4x4") {
      solve_board_impl<4>(SudokuBoards::BOARD_4x4, solver_type);
    } else if (board_name == "9x9") {
      solve_board_impl<9>(SudokuBoards::BOARD_9x9, solver_type);
    } else if (board_name == "9x9_extreme") {
      solve_board_impl<9>(SudokuBoards::BOARD_9x9_EXTREME, solver_type);
    } else {
      std::cerr
          << "Invalid board name. Must be '4x4', '9x9', or '9x9_extreme'.\n";
      return 1;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    print_usage(argv[0]);
    return 1;
  }

  return 0;
}

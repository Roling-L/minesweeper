#include "minesweepergame.h"
#include <algorithm>
#include <random>
#include <ctime>

MinesweeperGame::MinesweeperGame(int rows, int cols, int mines) {
    resetGame(rows, cols, mines);
}

void MinesweeperGame::resetGame(int rows, int cols, int mines) {
    m_rows = rows;
    m_cols = cols;
    m_mineCount = mines;
    m_remainingCells = rows * cols - mines;
    m_gameState = GameState::Playing;

    // 初始化游戏板
    m_mines = std::vector<std::vector<bool>>(rows, std::vector<bool>(cols, false));
    m_cellStates = std::vector<std::vector<CellState>>(rows, std::vector<CellState>(cols, CellState::Hidden));
    m_adjacentMines = std::vector<std::vector<int>>(rows, std::vector<int>(cols, 0));

    initializeBoard();
    emit gameUpdated();
}

void MinesweeperGame::initializeBoard() {
    placeMines();
    calculateAdjacentMines();
}

void MinesweeperGame::placeMines() {
    std::vector<std::pair<int, int>> positions;
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            positions.emplace_back(i, j);
        }
    }

    // 随机打乱位置并放置地雷
    std::shuffle(positions.begin(), positions.end(), std::mt19937(std::time(nullptr)));
    for (int i = 0; i < m_mineCount && i < static_cast<int>(positions.size()); ++i) {
        auto [row, col] = positions[i];
        m_mines[row][col] = true;
    }
}

void MinesweeperGame::calculateAdjacentMines() {
    for (int i = 0; i < m_rows; ++i) {
        for (int j = 0; j < m_cols; ++j) {
            if (m_mines[i][j]) {
                m_adjacentMines[i][j] = -1; // 地雷单元格
                continue;
            }

            int count = 0;
            for (int di = -1; di <= 1; ++di) {
                for (int dj = -1; dj <= 1; ++dj) {
                    if (di == 0 && dj == 0) continue;
                    int ni = i + di;
                    int nj = j + dj;
                    if (ni >= 0 && ni < m_rows && nj >= 0 && nj < m_cols && m_mines[ni][nj]) {
                        ++count;
                    }
                }
            }
            m_adjacentMines[i][j] = count;
        }
    }
}

// ... 其他成员函数的实现 ...

void MinesweeperGame::revealCell(int row, int col) {
    if (m_gameState != GameState::Playing ||
        row < 0 || row >= m_rows || col < 0 || col >= m_cols ||
        m_cellStates[row][col] != CellState::Hidden) {
        return;
    }

    if (m_mines[row][col]) {
        // 踩到地雷
        m_cellStates[row][col] = CellState::Revealed;
        m_gameState = GameState::Lose;
        emit gameLost();
        emit gameUpdated();
        return;
    }

    // 揭示单元格
    m_cellStates[row][col] = CellState::Revealed;
    --m_remainingCells;

    if (m_remainingCells == 0) {
        m_gameState = GameState::Win;
        emit gameWon();
    }

    // 如果周围没有地雷，自动揭示周围的单元格
    if (m_adjacentMines[row][col] == 0) {
        revealAdjacentCells(row, col);
    }

    emit gameUpdated();
}

void MinesweeperGame::revealAdjacentCells(int row, int col) {
    for (int di = -1; di <= 1; ++di) {
        for (int dj = -1; dj <= 1; ++dj) {
            if (di == 0 && dj == 0) continue;
            int ni = row + di;
            int nj = col + dj;
            if (ni >= 0 && ni < m_rows && nj >= 0 && nj < m_cols) {
                revealCell(ni, nj);
            }
        }
    }
}

void MinesweeperGame::toggleFlag(int row, int col) {
    if (m_gameState != GameState::Playing ||
        row < 0 || row >= m_rows || col < 0 || col >= m_cols) {
        return;
    }

    if (m_cellStates[row][col] == CellState::Hidden) {
        m_cellStates[row][col] = CellState::Flagged;
    }
    else if (m_cellStates[row][col] == CellState::Flagged) {
        m_cellStates[row][col] = CellState::Hidden;
    }

    emit gameUpdated();
}

// ... 其他简单成员函数的实现 ...
#pragma once
#include <vector>
#include <QObject>

class MinesweeperGame : public QObject {
    Q_OBJECT
public:
    MinesweeperGame(int rows = 10, int cols = 10, int mines = 15);

    // 游戏状态
    enum class CellState { Hidden, Revealed, Flagged };
    enum class GameState { Playing, Win, Lose };

    // 获取游戏状态
    GameState gameState() const;
    int rows() const;
    int cols() const;
    int mineCount() const;
    int remainingMines() const;

    // 获取单元格信息
    bool isMine(int row, int col) const;
    CellState cellState(int row, int col) const;
    int adjacentMines(int row, int col) const;

public slots:
    void revealCell(int row, int col);
    void toggleFlag(int row, int col);
    void resetGame(int rows = 10, int cols = 10, int mines = 15);

signals:
    void gameUpdated();
    void gameWon();
    void gameLost();

private:
    void initializeBoard();
    void placeMines();
    void calculateAdjacentMines();
    void revealAdjacentCells(int row, int col);

    int m_rows;
    int m_cols;
    int m_mineCount;
    int m_remainingCells;
    GameState m_gameState;

    std::vector<std::vector<bool>> m_mines;
    std::vector<std::vector<CellState>> m_cellStates;
    std::vector<std::vector<int>> m_adjacentMines;
};
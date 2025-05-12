#pragma once

#include <QMainWindow>
#include "minesweepergame.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onGameUpdated();
    void onGameWon();
    void onGameLost();
    void onNewGameTriggered();
    void onCellClicked(int row, int col);
    void onCellRightClicked(int row, int col);

private:
    void setupGameBoard();
    void updateGameBoard();
    void createMenu();

    Ui::MainWindow* ui;
    MinesweeperGame* m_game;
};
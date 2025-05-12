#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPushButton>
#include <QGridLayout>
#include <QMessageBox>
#include <QAction>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_game(new MinesweeperGame(this))
{
    ui->setupUi(this);

    // 设置游戏板
    setupGameBoard();

    // 创建菜单
    createMenu();

    // 连接信号和槽
    connect(m_game, &MinesweeperGame::gameUpdated, this, &MainWindow::onGameUpdated);
    connect(m_game, &MinesweeperGame::gameWon, this, &MainWindow::onGameWon);
    connect(m_game, &MinesweeperGame::gameLost, this, &MainWindow::onGameLost);

    // 初始更新
    onGameUpdated();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupGameBoard() {
    QWidget* centralWidget = new QWidget(this);
    QGridLayout* gridLayout = new QGridLayout(centralWidget);
    gridLayout->setSpacing(1);

    for (int row = 0; row < m_game->rows(); ++row) {
        for (int col = 0; col < m_game->cols(); ++col) {
            QPushButton* button = new QPushButton();
            button->setFixedSize(30, 30);
            button->setProperty("row", row);
            button->setProperty("col", col);

            connect(button, &QPushButton::clicked, [this, button]() {
                onCellClicked(button->property("row").toInt(), button->property("col").toInt());
                });

            // 右键点击标记
            button->setContextMenuPolicy(Qt::CustomContextMenu);
            connect(button, &QPushButton::customContextMenuRequested, [this, button]() {
                onCellRightClicked(button->property("row").toInt(), button->property("col").toInt());
                });

            gridLayout->addWidget(button, row, col);
        }
    }

    setCentralWidget(centralWidget);
}

void MainWindow::updateGameBoard() {
    QGridLayout* gridLayout = qobject_cast<QGridLayout*>(centralWidget()->layout());

    for (int row = 0; row < m_game->rows(); ++row) {
        for (int col = 0; col < m_game->cols(); ++col) {
            QPushButton* button = qobject_cast<QPushButton*>(gridLayout->itemAtPosition(row, col)->widget());

            switch (m_game->cellState(row, col)) {
            case MinesweeperGame::CellState::Hidden:
                button->setText("");
                button->setEnabled(true);
                button->setStyleSheet("background-color: lightgray;");
                break;
            case MinesweeperGame::CellState::Revealed:
                button->setEnabled(false);
                if (m_game->isMine(row, col)) {
                    button->setText("💣");
                    button->setStyleSheet("background-color: red;");
                }
                else {
                    int adjacent = m_game->adjacentMines(row, col);
                    if (adjacent > 0) {
                        button->setText(QString::number(adjacent));
                        // 可以根据数字设置不同颜色
                        switch (adjacent) {
                        case 1: button->setStyleSheet("color: blue;"); break;
                        case 2: button->setStyleSheet("color: green;"); break;
                        case 3: button->setStyleSheet("color: red;"); break;
                        default: button->setStyleSheet("color: darkred;"); break;
                        }
                    }
                    else {
                        button->setText("");
                    }
                    button->setStyleSheet(button->styleSheet() + "background-color: white;");
                }
                break;
            case MinesweeperGame::CellState::Flagged:
                button->setText("🚩");
                button->setStyleSheet("background-color: lightgray;");
                break;
            }
        }
    }

    setWindowTitle(QString("扫雷 - 剩余地雷: %1").arg(m_game->remainingMines()));
}

void MainWindow::createMenu() {
    QMenu* gameMenu = menuBar()->addMenu("游戏");

    QAction* newGameAction = new QAction("新游戏", this);
    connect(newGameAction, &QAction::triggered, this, &MainWindow::onNewGameTriggered);
    gameMenu->addAction(newGameAction);

    QAction* exitAction = new QAction("退出", this);
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);
    gameMenu->addAction(exitAction);
}

void MainWindow::onGameUpdated() {
    updateGameBoard();
}

void MainWindow::onGameWon() {
    QMessageBox::information(this, "恭喜", "你赢了！");
}

void MainWindow::onGameLost() {
    QMessageBox::information(this, "游戏结束", "你踩到地雷了！");
}

void MainWindow::onNewGameTriggered() {
    m_game->resetGame(10, 10, 15);
}

void MainWindow::onCellClicked(int row, int col) {
    m_game->revealCell(row, col);
}

void MainWindow::onCellRightClicked(int row, int col) {
    m_game->toggleFlag(row, col);
}
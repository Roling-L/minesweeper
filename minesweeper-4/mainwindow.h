#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void gameOption();
    void customOption();
    void gameState(int state);
private:
    void initMenu();

    QLabel *msgLabel;
};

#endif // MAINWINDOW_H

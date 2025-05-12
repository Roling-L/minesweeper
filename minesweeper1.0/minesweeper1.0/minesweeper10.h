#pragma once

#include <QtWidgets/QWidget>
#include "ui_minesweeper10.h"

class minesweeper10 : public QWidget
{
    Q_OBJECT

public:
    minesweeper10(QWidget *parent = nullptr);
    ~minesweeper10();

private:
    Ui::minesweeper10Class ui;
};

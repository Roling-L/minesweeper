#include "mainwindow.h"
#include "qminesweeper.h"
#include <QMenuBar>
#include <QMenu>
#include <QDebug>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(200,250);

    msgLabel = new QLabel(this);
    msgLabel->hide();

    QMineSweeper::createMap(this);
    connect(QMineSweeper::getEventMine(),SIGNAL(mineSweeperGameOver(int)),
            this,SLOT(gameState(int)));
    initMenu();

}

MainWindow::~MainWindow()
{

}

void MainWindow::gameOption()
{
    QObject *option = qobject_cast<QObject *>(sender());
    QString objectname = option->objectName();

    if(objectname.contains("gameOption:"))
    {
        QStringList list= objectname.split(":");
        int r = list.at(1).toInt();
        int c = list.at(2).toInt();
        int mine = list.at(3).toInt();
        int minSize = 30;
        int x0 = 30;
        int y0 = 30;
        QMineSweeper::createMap(this,r,c,mine);
        connect(QMineSweeper::getEventMine(),SIGNAL(mineSweeperGameOver(int)),
                this,SLOT(gameState(int)));
        resize((c+1)*minSize+x0,(r+1)*minSize+y0);
        msgLabel->hide();
    }


}

void MainWindow::customOption()
{
    QGridLayout* gLayout = new QGridLayout;
    QDialog *dialog = new QDialog(this);

    QPushButton* pButton1 = new QPushButton(QString("确定"));
    QPushButton* pButton2 = new QPushButton(QString("取消"));
    pButton1->setFixedSize(50,30);
    pButton2->setFixedSize(50,30);
    QLabel* labe1 = new QLabel("行数:",dialog);
    QLabel* labe2 = new QLabel("列数:",dialog);
    QLabel* labe3 = new QLabel("雷数:",dialog);


    QLineEdit* edit1 = new QLineEdit(dialog);
    edit1->setPlaceholderText("输入地图的行数R");
    QLineEdit* edit2 = new QLineEdit(dialog);
    edit2->setPlaceholderText("输入地图的列数C");
    QLineEdit* edit3 = new QLineEdit(dialog);
    edit3->setPlaceholderText("输入雷的个数num<R*C");


    gLayout->addWidget(labe1,0,0);
    gLayout->addWidget(labe2,1,0);
    gLayout->addWidget(labe3,2,0,1,1);
    gLayout->addWidget(edit1,0,1,1,4);
    gLayout->addWidget(edit2,1,1,1,4);
    gLayout->addWidget(edit3,2,1,1,4);
    gLayout->addWidget(pButton1,3,1);
    gLayout->addWidget(pButton2,3,4);


    connect(pButton1,SIGNAL(clicked()),dialog,SLOT(accept()));
    connect(pButton2,SIGNAL(clicked()),dialog,SLOT(reject()));

    dialog->setWindowTitle("自定义");
    dialog->setMinimumSize(200,50);
    dialog->setWindowFlag(Qt::Dialog);
    dialog->setLayout(gLayout);

    if(QDialog::Accepted == dialog->exec())
    {
        int r = edit1->text().toInt();
        int c = edit2->text().toInt();
        int num = edit3->text().toInt();
        int minSize = 30;
        int x0 = 30;
        int y0 = 30;

        if(num > r*c)
            return;

        QMineSweeper::createMap(this,r,c,num,x0,y0,minSize);
        connect(QMineSweeper::getEventMine(),SIGNAL(mineSweeperGameOver(int)),
                this,SLOT(gameState(int)));

        resize((c+1)*minSize+x0,(r+1)*minSize+y0);
        msgLabel->hide();
    }
}

void MainWindow::gameState(int state)
{
    QString msg;

    if(state == GAME_STATE_SUCCESSFUL)
        msg = "恭喜通过！";
    else if(state == GAME_STATE_DEFEATED)
        msg = "游戏结束！";

    if(!msg.isEmpty())
    {
        msgLabel->setText(msg);
        msgLabel->setGeometry(100,100,100,20);
        msgLabel->raise();
        msgLabel->show();
    }
}

void MainWindow::initMenu()
{

    QMenuBar *bar = this->menuBar();
    QMenu *gameMenu = bar->addMenu("游戏模式");

    QAction*act = gameMenu->addAction("入门");
    act->setObjectName("gameOption:6:5:3");
    connect(act,SIGNAL(triggered()),this,SLOT(gameOption()));

    act = gameMenu->addAction("简单");
    act->setObjectName("gameOption:10:10:15");
    connect(act,SIGNAL(triggered()),this,SLOT(gameOption()));

    act = gameMenu->addAction("一般");
    act->setObjectName("gameOption:16:16:40");
    connect(act,SIGNAL(triggered()),this,SLOT(gameOption()));

    act = gameMenu->addAction("困难");
    act->setObjectName("gameOption:15:20:60");
    connect(act,SIGNAL(triggered()),this,SLOT(gameOption()));

    act = gameMenu->addAction("自定义");
    connect(act,SIGNAL(triggered()),this,SLOT(customOption()));

}

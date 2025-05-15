#include "qminesweeper.h"

#include <QDebug>
#include <QDateTime>
#include <QPainter>

QMap<int,QMineSweeper *> QMineSweeper::mineMap;
QWidget * QMineSweeper::mainWindow = nullptr;
QMineSweeper * QMineSweeper::eventMine = nullptr;
QString QMineSweeper::numColorStyle[10];
QString QMineSweeper::numStateStyle[7];

int QMineSweeper::mapRow = 6;
int QMineSweeper::mapCol = 5;
int QMineSweeper::mineNum = 3;
int QMineSweeper::mineSize = 30;
int QMineSweeper::gameState = GAME_STATE_SWEEP;
int QMineSweeper::mapX0 = 30;
int QMineSweeper::mapY0 = 30;
int QMineSweeper::openCount = 0;

void myrand(int n,int min,int max,QList<int> &list);

QMineSweeper::QMineSweeper(int id,QWidget *parent) : QBlock(parent)
{
    selfid = id;
    selfN = 0;
    selfState = BLOCK_STATE_AREA;
    mineMap.insert(id,this);
    updateStyle();

    if(eventMine == nullptr)
        eventMine = this;

   connect(this,SIGNAL(mineSweeperGameOver(int)),eventMine,SLOT(transponder(int)));
}

void QMineSweeper::createMap(QWidget *w, int row, int col, int mine, int x0, int y0, int mine_size)
{

    mainWindow = w;
    mapRow = row;
    mapCol = col;
    mineNum = mine;
    mapX0 = x0;
    mapY0 = y0;
    mineSize = mine_size;
    gameState = GAME_STATE_SWEEP;
    openCount = 0;
    eventMine = nullptr;

    numColorStyle[0] = "";
    numColorStyle[1] = "color: rgb(0, 147, 62);";
    numColorStyle[2] = "color: rgb(0, 187, 187);";
    numColorStyle[3] = "color: rgb(110, 40, 0);";
    numColorStyle[4] = "color: rgb(166, 19, 188);";
    numColorStyle[5] = "color: rgb(185, 122, 87);";
    numColorStyle[6] = "color: rgb(136, 0, 21);";
    numColorStyle[7] = "color: rgb(163, 73, 164);";
    numColorStyle[8] = "color: rgb(0, 0, 0);";
    numColorStyle[9] = "";

    numStateStyle[0] = "background-color: rgb(0, 120, 215);";
    numStateStyle[1] = "background-color: rgb(200, 200, 255);";
    numStateStyle[2] = "background-color: rgb(130, 255, 150);";
    numStateStyle[3] = "background-color: rgb(77, 166, 255);";
    numStateStyle[4] = "background-color: rgb(255, 0, 0);";
    numStateStyle[5] = "background-color: rgb(0, 170, 0);";
    numStateStyle[6] = "background-color: rgb(0, 170, 0);";


    initMap();

}

void QMineSweeper::initMap()
{
    qDeleteAll(mineMap);
    mineMap.clear();

    //生成雷方块的id列表
    QList<int> id_list;
    myrand(mineNum,0,mapRow * mapCol,id_list);

    for(int i = 0 ; i < mapRow * mapCol ; i++)
    {
            int id = i;
            QMineSweeper *block = new QMineSweeper(id,mainWindow);

            int r = i / mapCol;
            int c = i % mapCol;

            //如果该方块id在雷方块id列表内则赋值为雷(9)
            if(id_list.contains(id))
                block->setSelfN(9);

            //生成相邻方块的id列表
            for(int i_r = -1 ; i_r < 2 ; i_r ++)
                for(int i_c = -1 ; i_c < 2 ; i_c++)
                {
                    int nid = id + i_r * mapCol + i_c;
                    if(i_r+r>=0&&i_r+r<mapRow&&i_c+c>=0&&i_c+c<mapCol&&nid!=id)
                        block->neighbors.append(nid);
                }

            block->setGeometry(c*mineSize+mapX0,r*mineSize+mapY0,mineSize,mineSize);
            block->show();
    }

    //为雷方块相邻的方块映射雷区
    foreach(int mid, id_list)
    {
        foreach(int nid , mineMap[mid]->neighbors)
        {

            int n = mineMap[nid]->getSelfN();
            //n不为9表明为雷区，危险数值加一
            if(n != 9)
                mineMap[nid]->setSelfN(n+1);
        }

    }
}

void QMineSweeper::transponder(int state)
{
    QMineSweeper *sender_p = qobject_cast<QMineSweeper *>(sender());

    //若信号发出者不是自身则转发事件信号
    if(sender_p != eventMine)
        emit mineSweeperGameOver(state);

}

void QMineSweeper::openBlock()
{
    if(selfState == BLOCK_STATE_OPENED)
        return;
    if(selfN == 9)
    {
        gameState = GAME_STATE_DEFEATED;
        selfState = BLOCK_STATE_BOOM;
        updateStyle();
        emit mineSweeperGameOver(gameState);
        return;
    }

    selfState = BLOCK_STATE_OPENED;
    openCount++;
    updateStyle();

    emit mineSweeperGameOver(GAME_STATE_SWEEP);

    if(openCount >= mapRow*mapCol - mineNum)
    {
        gameState = GAME_STATE_SUCCESSFUL;
        emit mineSweeperGameOver(gameState);
        return;
    }

    if(selfN == 0)
    {
        foreach(int id , neighbors)
            mineMap[id]->openBlock();
    }

}

#include <QStyle>

void QMineSweeper::updateStyle()
{
    if(selfN != 0 && selfN !=9 && selfState == BLOCK_STATE_OPENED)
        this->setText(QString("%1").arg(selfN));
    else
         this->setText(QString(" "));



    setStyleSheet(numStateStyle[selfState]+numColorStyle[selfN]);
    this->update();
}

void QMineSweeper::mousePressEvent(QMouseEvent *event)
{
    if(gameState != GAME_STATE_SWEEP)
        return;

    if(event->button() == Qt::RightButton)
    {
        if(selfState == BLOCK_STATE_OPENED)
            return;
        else if(selfState == BLOCK_STATE_AREA)
        {
            selfState = BLOCK_STATE_FLAG;
           emit mineSweeperGameOver(GAME_SET_FLAG);
        }
        else if(selfState == BLOCK_STATE_FLAG)
        {
            selfState = BLOCK_STATE_DOUBT;
            emit mineSweeperGameOver(GAME_RESET_FLAG);
        }
        else if(selfState == BLOCK_STATE_DOUBT)
        {
            selfState = BLOCK_STATE_AREA;
        }
        updateStyle();

    }
    else if(event->button() == Qt::LeftButton)
    {
        openBlock();

    }
}



void myrand(int n,int min,int max,QList<int> &list)
{
    QMap<int,bool> maps;

    qsrand((uint)QDateTime::currentMSecsSinceEpoch());

    for(int i = 0 ; i < n && i < max - min; i++)
    {
        int r_count = qrand()%(max-min)+min;
        while(maps[r_count] == true)
            r_count = qrand()%(max-min)+min;

        maps[r_count] = true;
        list.append(r_count);

    }
}

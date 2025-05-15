#ifndef QMINESWEEPER_H
#define QMINESWEEPER_H

#include <QWidget>
#include <QMap>
#include <QPushButton>
#include <QMouseEvent>


//方块状态码
#define BLOCK_STATE_AREA    0       //未打开
#define BLOCK_STATE_OPENED  1       //已打开
#define BLOCK_STATE_FLAG    2       //被标记
#define BLOCK_STATE_DOUBT   3       //被怀疑
#define BLOCK_STATE_BOOM    4       //被引爆
#define BLOCK_STATE_SWEEPED 5       //被扫除
#define BLOCK_STATE_MISTAKE 6       //标记错误

//游戏状态信号码
#define GAME_STATE_SWEEP        0   //有方块被扫除
#define GAME_STATE_SUCCESSFUL   1   //所有方块被扫除游戏通过
#define GAME_STATE_DEFEATED     2   //游戏结束
#define GAME_SET_FLAG           3   //有方块被标记
#define GAME_RESET_FLAG         4   //有方块被取消标志


typedef QPushButton QBlock;


class QMineSweeper : public QBlock
{
    Q_OBJECT
public:
    explicit QMineSweeper(int id,QWidget *parent = nullptr);

    static void createMap(QWidget *w = mainWindow ,int row = mapRow ,int col = mapCol,int mine = mineNum , int x0 = mapX0, int y0 = mapY0 ,int mine_size = mineSize);
    static void setMapSize(int row,int col){mapRow = row; mapCol = col;}
    static void setMapXY0(int x0 ,int y0){mapX0 = x0 ; mapY0 = y0;}
    static void setMineNum(int num){mineNum = num;}
    static void setMineSize(int size){mineSize = size;}
    static void setMainWindow(QWidget *w){mainWindow = w;}
    static void initMap();
    static QMineSweeper *getEventMine(){return eventMine;}//eventMine方块将所有的方块信号进行转发
private:
    //地图与地雷尺寸
    static QMap<int,QMineSweeper *> mineMap;
    static int mapRow;
    static int mapCol;
    static int mineNum;
    static int mineSize;
    static int gameState;
    static int mapX0;
    static int mapY0;
    static int openCount;//方块被打开的数量统计
    static QString numColorStyle[10];//0~9的颜色状态样式列表
    static QString numStateStyle[7];//方块状态码对应的样式列表
    static QWidget *mainWindow;     //主窗口
    static QMineSweeper *eventMine; //用于转发所有事件的方块


signals:
    void mineSweeperGameOver(int state);//发送游戏状态的信号
private slots:
    void transponder(int state);//eventMine方块将所有的方块信号转发

private:
    void setSelfN(int n){selfN = n;}
    int  getSelfN(){return selfN;}
    void openBlock();
    void updateStyle();

public:

protected:
    virtual void mousePressEvent(QMouseEvent *event);


private:

    int selfid;     //方块唯一标识id
    int selfN;      //危险数值
    int selfState;
    QList<int> neighbors;

};

#endif // QMINESWEEPER_H

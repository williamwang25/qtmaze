#ifndef MAZE_H
#define MAZE_H
#include <iostream>
#include <QWidget>
#include <QMessageBox>      //Qt信息窗口头文件
#include <QPainter>         //Qt绘图头文件
#include <QDebug>           //QtDebug头文件
#include <QKeyEvent>        //Qt按键事件头文件
#include <QTimer>           //Qt计时器头文件
#include <QInputDialog>     //Qt输入对话框头文件

#include "stack.h"
#include "queue.h"
/**************************************************
 * -1:边界 0:墙壁
 * 1:空单元(初始化后未判定的单元)
 * 2:待定墙壁(新建已定单元时建立)
 * 3:已定单元(确定迷宫树已延伸到的单元)
 * 4:打通墙壁(延伸操作)
 * 5.起点
 * 6.终点
 * 7.已经过路径
 * 8.已搜索路径
 * 9.当前搜索路径
 * **************************************************/
 enum class CellType {
    Border = -1,
    Wall = 0,
    EmptyCell = 1,
    PendingWall = 2,
    Path = 3,
    OpenWall = 4,
    Start = 5,
    End = 6,
    VisitedPath = 7,
    SearchedPath = 8,
    CurrentSearch = 9
};


struct Pos{
    int i;
    int j;
};

struct PosPath {
    int i;
    int j;
    PosPath* prev;  // 前驱结点指针
    
    PosPath(int x, int y, PosPath* p = nullptr) : i(x), j(y), prev(p) {}
};

class maze :public QObject{
    Q_OBJECT
private:
    bool foundpath = false; // 是否找到路径
    bool isPaused = false; // 是否暂停
    bool isExit = false; // 是否退出

    int delay;
    int level;  //迷宫阶数
    int** map;  //地图存储空间
    int start_x, start_y;   //起终点
    int end_x,end_y;
    //生成基础地图（单元格）
    void base();
    //使点的周围设为待定（2）
    void initCell(int i, int j);
    //设定迷宫开始延伸的起点
    void start() ;
    //循环停止判定（是否存在未判定的区域）
    bool judge();
    //操作（如果相邻空单元（1）则打通（变为4），如果不相邻空单元则为墙壁（0））
    void updateCell(int i, int j);
    //随机选择一个待定墙壁判断并操作
    void randomCell();

    bool competeMode;     // 是否为竞赛模式
    Stack<Pos> dfsStack;  // DFS用的栈
    Queue<PosPath*> bfsQueue; // BFS用的队列


public:
    //构造函数申请内存空间
    maze(int in_level);
    ~maze();
    //获取地图
    int getlevel();
    int** getmap();
    int getside();
    //生成地图
    void makemap();
    int p_x, p_y; //当前位置
    int dfs_x, dfs_y;   // DFS算法位置
    int bfs_x, bfs_y;   // BFS算法位置
    // 算法是否在运行
    bool dfsRunning;
    bool bfsRunning;

    enum Winner {
        None,
        Player,
        DFS,
        BFS
    };

    Winner winner;

    // 获取三个角色的位置
    int getPlayerX() const { return p_x; }
    int getPlayerY() const { return p_y; }
    int getDfsX() const { return dfs_x; }
    int getDfsY() const { return dfs_y; }
    int getBfsX() const { return bfs_x; }
    int getBfsY() const { return bfs_y; }
    
    // 重置位置
    void resetPositions();
    
    // 竞赛模式DFS单步移动
    bool moveDfsOneStep();
    
    // 竞赛模式BFS单步移动
    bool moveBfsOneStep();
    
    // 设置竞赛模式
    void setCompeteMode(bool mode);
    bool isCompeteMode() const { return competeMode; }

    Pos x[1000];
    int p[100][100];
    bool able(int k,int t);
    void brush(int k);
    void search(int k);
    bool dfs(int k);
    void solve();
    void setPause(bool state); // 设置暂停
    void setExit(bool state); // 设置退出
    bool getPauseState() const; // 获取暂停状态
    void resetState(); // 重置状态
    
    //重置地图
    void rebuildmap();
    int* operator[](int index);
    //搜索速度
    void setspeed(int ms);

    // 链栈DFS
    void dfs_stack();
    // 链队BFS
    void bfs_queue();


signals:
    void mazeUpdated();
    void searchOver();
    void competitionOver(int winner); // 添加竞赛结束信号

    
};


#endif // MAZE_H

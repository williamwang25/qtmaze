#include <QThread>
#include "maze.h"
#include "stack.h"
#include "queue.h"
#include <qcoreapplication.h>


//生成基础地图（单元格）
void maze::base() {
    for (int i = 0; i < level * 2 + 1; i++) {
        for (int j = 0; j < level * 2 + 1; j++) {
            if (i == 0 || j == 0 || i == level * 2 + 1 - 1 ||
                j == level * 2 + 1 - 1) {
                map[i][j] = -1;
            } else if (i % 2 != 0 && j % 2 != 0) {
                map[i][j] = 1;
            } else {
                map[i][j] = 0;
            }
        }
    }
}
//使点的周围设为待定（2）
void maze:: initCell(int i, int j) {
    if (map[i - 1][j] == 0) {
        map[i - 1][j] = 2;
    }
    if (map[i + 1][j] == 0) {
        map[i + 1][j] = 2;
    }
    if (map[i][j - 1] == 0) {
        map[i][j - 1] = 2;
    }
    if (map[i][j + 1] == 0) {
        map[i][j + 1] = 2;
    }
}
//设定迷宫开始延伸的起点
void maze:: start() {
    map[start_x][start_y] = 5;
    initCell(start_x, start_y);
}
//循环停止判定（是否存在未判定的区域）
bool maze:: judge() {
    bool flag = 0;
    for (int i = 0; i < level * 2 + 1; i++) {
        for (int j = 0; j < level * 2 + 1; j++) {
            if (map[i][j] == 2) {
                flag = 1;
                return flag;
            }
        }
    }
    return flag;
}
//操作（如果相邻空单元（1）则打通（变为4），如果不相邻空单元则为墙壁（0））
void maze:: updateCell(int i, int j) {
    if ((map[i - 1][j] == 3 || map[i - 1][j] == 5) && map[i + 1][j] == 1) {
        map[i][j] = 4;
        map[i + 1][j] = 3;
        initCell(i + 1, j);
        start_x = i + 1;
        start_y = j;
    } else if ((map[i][j - 1] == 3 || map[i][j - 1] == 5) &&
               map[i][j + 1] == 1) {
        map[i][j] = 4;
        map[i][j + 1] = 3;
        initCell(i, j + 1);
        start_x = i;
        start_y = j + 1;
    } else if ((map[i + 1][j] == 3 || map[i + 1][j] == 5) &&
               map[i - 1][j] == 1) {
        map[i][j] = 4;
        map[i - 1][j] = 3;
        initCell(i - 1, j);
        start_x = i - 1;
        start_y = j;
    } else if ((map[i][j + 1] == 3 || map[i][j + 1] == 5) &&
               map[i][j - 1] == 1) {
        map[i][j] = 4;
        map[i][j - 1] = 3;
        initCell(i, j - 1);
        start_x = i;
        start_y = j - 1;
    } else {
        map[i][j] = 0;
    }
}
//随机选择一个待定墙壁判断并操作
void maze:: randomCell() {
    int t = 0;
    for (int i = 0; i < level * 2 + 1; i++) {
        for (int j = 0; j < level * 2 + 1; j++) {
            if (map[i][j] == 2) {
                t++;
            }
        }
    }
    int k = rand() % t + 1;
    t = 0;
    for (int i = 0; i < level * 2 + 1; i++) {
        for (int j = 0; j < level * 2 + 1; j++) {
            if (map[i][j] == 2) {
                t++;
                if (t == k) {
                    updateCell(i, j);
                    goto loopout;
                }
            }
        }
    }
loopout:
    if (!judge()) {
        map[start_x][start_y] = 6;
    }
}

//构造函数申请内存空间
maze:: maze(int in_level) : level(in_level) {
    map = new int* [level * 2 + 1];
    for (int i = 0; i < level * 2 + 1; i++) {
        map[i] = new int[level * 2 + 1];
    }
    start_x = 1, start_y = 1; //起点设置为(1,1)

    isPaused = false;
    isExit = false;
}
maze::~maze() {
    for (int i = 0; i < level * 2 + 1; i++) {
        delete [] map[i];
    }
    delete [] map;
}

void maze::setPause(bool state) {
    isPaused = state;
}

void maze::setExit(bool state) {
    isExit = state;
}

bool maze::getPauseState() const{
    return isPaused;
}

void maze::resetState() {
    isPaused = false;
    isExit = false;
}


//获取地图
int maze:: getlevel() {
    return maze::level;
}
int** maze::getmap() {
    return map;
}
int maze::getside() {
    return level * 2 + 1;
}
//生成地图
void maze:: makemap() {
    p_x = start_x;
    p_y = start_y;
    base();
    start();
    int a = 0;
    while (judge()) {
        a++;
        randomCell();
        // if (a % 30 == 0) {
        //     printarr(map, level);
        //     system("PAUSE");
        // }
    }
}
int* maze::operator[](int index) {
    return map[index];
}
//重置地图
void maze::rebuildmap() {
    start_x = 1;
    start_y = 1;
    makemap();
}
bool maze::able(int k,int t){
    Pos last=x[k-1];
    Pos cur=last;
    if(t==1&&p[cur.i][cur.j+1]!=0)return 0;
    if(t==2&&p[cur.i+1][cur.j]!=0)return 0;
    if(t==3&&p[cur.i][cur.j-1]!=0)return 0;
    if(t==4&&p[cur.i-1][cur.j]!=0)return 0;

    return 1;
}
void maze::brush(int k){
    for(int m=0;m<k;m++){
        map[x[m].i][x[m].j]=7;
    }
    map[x[k-1].i][x[k-1].j]=6;
}

//搜索方向数组
const int dx[4] = {0, 1, 0, -1};  // 右、下、左、上
const int dy[4] = {1, 0, -1, 0};

//深度优先搜索
bool maze::dfs(int k) {
    if(foundpath) return true;
    
    map[x[k-1].i][x[k-1].j] = 9;
    emit mazeUpdated();
    
    if(x[k-1].i == end_x && x[k-1].j == end_y) {
        brush(k);
        emit searchOver();
        foundpath = true;
        return true;
    }
    
    // 使用方向数组简化代码
    for(int dir = 0; dir < 4; dir++) {
        int ni = x[k-1].i + dx[dir];
        int nj = x[k-1].j + dy[dir];
        
        if(p[ni][nj] == 0) {  // 如果可以移动
            x[k] = {ni, nj};
            p[ni][nj] = 2;  // 标记为已访问
            dfs(k+1);
            
            if(!foundpath && map[ni][nj] != 7 && map[ni][nj] != 6) {
                map[ni][nj] = 8;
                emit mazeUpdated();
            }
            
            p[ni][nj] = 0;
        }
    }
    
    if(!foundpath && map[x[k-1].i][x[k-1].j] == 9) {
        map[x[k-1].i][x[k-1].j] = 8;
    }
    
    return false;
}

void maze::solve(){
    resetState();
    foundpath=false;
    for(int i=0;i<level*2+1;i++){
        for(int j=0;j<level*2+1;j++){
            if(map[i][j]==0||map[i][j]==-1)p[i][j]=1;
            else if(map[i][j]==6){p[i][j]=0;end_x=i;end_y=j;}
            else p[i][j]=0;
        }

    }
    Pos start={p_x,p_y};
    x[0]=start;
    dfs(1);

    emit searchOver();

    return;
}

// 栈DFS
void maze::dfs_stack() {
    resetState();//重置状态
    foundpath = false;
    // 初始化访问标记数组
    for(int i = 0; i < level*2+1; i++) {
        for(int j = 0; j < level*2+1; j++) {
            if(map[i][j] == 0 || map[i][j] == -1) {
                p[i][j] = 1; // 墙壁和边界不可访问
            } else if(map[i][j] == 6) {
                p[i][j] = 0; // 终点可访问
                end_x = i;
                end_y = j;
            } else {
                p[i][j] = 0; // 其他位置可访问
            }
        }
    }
    
    Stack<Pos> stack;
    
    // 起点入栈
    Pos start = {p_x, p_y};
    stack.push(start);
    p[p_x][p_y] = 2; // 标记为已访问
    
    // 方向数组
    const int dx[4] = {0, 1, 0, -1};  // 右、下、左、上
    const int dy[4] = {1, 0, -1, 0};
    
    while(!stack.isEmpty() && !foundpath && !isExit) {
        
        while(isPaused && !isExit) {
            QThread::msleep(100); // 暂停时小睡，减少CPU使用率
            QCoreApplication::processEvents(); // 允许处理其他事件
        }
        
        if(isExit) {
            emit searchOver(); // 如果被终止，发出搜索结束信号
            return;
        }
        
        // 取出栈顶元素但不弹出
        Pos current = stack.peek();
        
        // 标记当前位置为搜索中
        map[current.i][current.j] = 9; // 当前搜索路径
        emit mazeUpdated(); // 更新界面
        
        // 延时显示搜索过程
        QThread::msleep(delay);
        
        // 检查是否到达终点
        if(current.i == end_x && current.j == end_y) {
            foundpath = true;
            
            // 回溯路径并标记为已访问路径
            Stack<Pos> path;
            while(!stack.isEmpty()) {
                Pos pos = stack.pop();
                path.push(pos);
            }
            
            while(!path.isEmpty()) {
                Pos pos = path.pop();
                map[pos.i][pos.j] = 7; // 已经过路径
            }
            
            map[end_x][end_y] = 6; // 确保终点标记不变
            emit mazeUpdated();
            emit searchOver();
            return;
        }
        
        // 尝试四个方向
        bool found = false;
        for(int dir = 0; dir < 4; dir++) {
            int ni = current.i + dx[dir];
            int nj = current.j + dy[dir];
            
            // 检查是否可以移动到下一个位置
            if(p[ni][nj] == 0) {
                // 可以移动
                Pos next = {ni, nj};
                stack.push(next);
                p[ni][nj] = 2; // 标记为已访问
                found = true;
                break; // 找到一个可行方向就继续深入
            }
        }
        
        // 如果四个方向都不可行，回溯
        if(!found) {
            Pos backtrack = stack.pop();
            if(map[backtrack.i][backtrack.j] == 9 && 
               backtrack.i != end_x && backtrack.j != end_y) {
                map[backtrack.i][backtrack.j] = 8; // 标记为已搜索路径
                emit mazeUpdated();
            }
        }
    }
    
    if(!foundpath) {
        // 没有找到路径
        emit searchOver();
    }
}

// 链队BFS
void maze::bfs_queue() {
    resetState(); 
    foundpath = false;
    
    // 初始化访问标记数组
    for(int i = 0; i < level*2+1; i++) {
        for(int j = 0; j < level*2+1; j++) {
            if(map[i][j] == 0 || map[i][j] == -1) {
                p[i][j] = 1; // 墙壁和边界不可访问
            } else if(map[i][j] == 6) {
                p[i][j] = 0; // 终点可访问
                end_x = i;
                end_y = j;
            } else {
                p[i][j] = 0; // 其他位置可访问
            }
        }
    }
    
    // 创建一个队列用于BFS
    Queue<PosPath*> queue;
    
    // 起点入队
    PosPath* start = new PosPath(p_x, p_y);
    queue.enqueue(start);
    p[p_x][p_y] = 2; // 标记为已访问
    
    // 方向数组
    const int dx[4] = {0, 1, 0, -1};  // 右、下、左、上
    const int dy[4] = {1, 0, -1, 0};
    
    PosPath* endPos = nullptr; // 用于存储找到的终点路径
    
    while(!queue.isEmpty() && !foundpath && !isExit) {
        // 非阻塞式事件处理
        QCoreApplication::processEvents();
        // 添加暂停检查
        while(isPaused && !isExit) {
            QThread::msleep(100); 
            QCoreApplication::processEvents(); // 允许处理其他事件
        }
        
        if(isExit) {
            // 清理队列中的所有元素
            while(!queue.isEmpty()) {
                delete queue.dequeue();
            }
            emit searchOver(); // 如果被终止，发出搜索结束信号
            return;
        }

        // 取出队头元素
        PosPath* current = queue.dequeue();
        
        // 标记当前位置为搜索中
        map[current->i][current->j] = 9; // 当前搜索路径
        emit mazeUpdated(); // 更新界面
        
        // 延时显示搜索过程
        QThread::msleep(delay);
        
        // 检查是否到达终点
        if(current->i == end_x && current->j == end_y) {
            foundpath = true;
            endPos = current; // 保存终点路径
            break;
        }
        
        // 尝试四个方向
        for(int dir = 0; dir < 4; dir++) {
            int ni = current->i + dx[dir];
            int nj = current->j + dy[dir];
            
            // 检查是否可以移动到下一个位置
            if(p[ni][nj] == 0) {
                // 可以移动
                PosPath* next = new PosPath(ni, nj, current);
                queue.enqueue(next);
                p[ni][nj] = 2; // 标记为已访问
                
                // 标记为已搜索路径
                if(map[ni][nj] != 6) { // 不是终点
                    map[ni][nj] = 8; // 已搜索路径
                }
                emit mazeUpdated();
            }
        }
    }
    
    // 如果找到路径，回溯并标记
    if(foundpath && endPos != nullptr) {
        // 回溯路径并标记为已经过路径
        PosPath* current = endPos;
        while(current != nullptr) {
            if(map[current->i][current->j] != 5 && map[current->i][current->j] != 6) {
                map[current->i][current->j] = 7; // 已经过路径
            }
            PosPath* temp = current;
            current = current->prev;
            delete temp; // 释放内存
        }
        
        map[end_x][end_y] = 6; // 确保终点标记不变
        emit mazeUpdated();
    } else {
        // 清理队列中的所有元素
        while(!queue.isEmpty()) {
            delete queue.dequeue();
        }
    }
    
    emit searchOver();
}

void maze::setspeed(int ms){
    delay = ms;
}

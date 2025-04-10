#include <QObject>
#include <QColor>
#include <QInputDialog>
#include "rank.h"
#include "mazewidget.h"
#include "ui_mazewidget.h"

mazeWidget::mazeWidget(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::mazeWidget), map(new maze(20)) // 将 maze 实例传递给 solve 的构造函数
    , painting_switch(false), timing_switch(false)
    , keybord_switch(false), stop_switch(false), grade(0), time(0), competeMode(false) {
    //TODO:状态栏
    ui->setupUi(this);
    ui->progressBar->setVisible(false);                 //初始隐藏进度条
    ui->end_btn->setEnabled(false);                     //设置终止按钮禁用
    ui->stop_ptn->setEnabled(false);                    //设置暂停按钮禁用
    ui->grade_value->setText("  ");                     //设置分数值显示为空
    ui->time_value->setText("  ");                      //设置时间值显示为空
    map->makemap();                                     //生成地图
    timer = new QTimer(this);                                          //初始化计时器
    connect(timer, &QTimer::timeout, this, &mazeWidget::time_update);   //链接时间更新信号与槽
    ui->plaque_time->setText("  ");
    ui->plaque_grade->setText("  ");
    ui->label->setVisible(true);
    ui->solve_btn->setEnabled(false);

    // 初始化竞赛计时器，设置较长的间隔，便于观察
    competeTimer = new QTimer(this);
    connect(competeTimer, &QTimer::timeout, this, &mazeWidget::competitionStepUpdate);
    connect(map, &maze::competitionOver, this, &mazeWidget::onCompetitionOver);
    
    // 默认隐藏竞赛按钮，在race_btn点击时显示
    ui->compete_button->setVisible(false);

}

mazeWidget::~mazeWidget(){
    delete ui;
    delete map;
    delete timer;
    delete competeTimer;
}
void mazeWidget::paintEvent(QPaintEvent*) {
    if((!painting_switch) && (!competeMode)) return;
    
    QPainter painter(this);

    
    int perblock = (std::min(ui->frame->width(), ui->frame->height()) - 20) / (map->getside());
    int start_x = ui->frame->x() + (ui->frame->width() - (ui->frame->x() + (map->getside()) * perblock)) / 2;
    int strat_y = ui->frame->y() + (ui->frame->height() - (ui->frame->y() + (map->getside()) * perblock)) / 2;
    // 绘制基本地图元素（墙壁、空白路径等）
    for(int i = 0; i < map->getlevel() * 2 + 1; i++) {
        for(int j = 0; j < map->getlevel() * 2 + 1; j++) {
            if(map->getmap()[i][j] == 3 || map->getmap()[i][j] == 4) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::white));
            } else if(map->getmap()[i][j] == 5) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::blue));
            } else if(map->getmap()[i][j] == 6) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::green));
            } else if(map->getmap()[i][j] == 0 || map->getmap()[i][j] == -1) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(QColor("#5bc0e8")));
            }
        }
    }
    
    // 在竞赛模式下，绘制算法路径
    if(competeMode) {
        for(int i = 0; i < map->getlevel() * 2 + 1; i++) {
            for(int j = 0; j < map->getlevel() * 2 + 1; j++) {
                if(map->getmap()[i][j] == 8) {
                    //BFS已搜索路径
                    painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::lightGray));
                } else if(map->getmap()[i][j] == 9) {
                    //DFS已搜索路径
                    painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(QColor("#f1e2f3")));
                }else if(map->getmap()[i][j] == 10) {
                    // BFS搜索前沿
                    painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::cyan));
                }
            }
        }
    }
    
    // 绘制玩家路径（总是在最上层）
    for(int i = 0; i < map->getlevel() * 2 + 1; i++) {
        for(int j = 0; j < map->getlevel() * 2 + 1; j++) {
            if(map->getmap()[i][j] == 7) {
                painter.fillRect(start_x + i * perblock, strat_y + j * perblock, perblock, perblock, QBrush(Qt::yellow));
            }
        }
    }
    
    // 绘制三个实体
    // 在三个实体的位置上绘制半透明的标记
    // 玩家 - 红色
    painter.fillRect(start_x + map->p_x * perblock, strat_y + map->p_y * perblock, perblock, perblock, QBrush(Qt::red));

    // 在竞赛模式下显示算法位置
    if(competeMode) {
        // DFS - 深紫色，如果玩家和DFS位置重叠，绘制半透明标记
        if(map->dfsRunning) {
            if(map->p_x == map->getDfsX() && map->p_y == map->getDfsY()) {
                // 绘制红色与紫色的混合颜色
                QColor mixedColor(Qt::darkMagenta);
                mixedColor.setAlpha(128); // 半透明
                painter.fillRect(start_x + map->getDfsX() * perblock, strat_y + map->getDfsY() * perblock, 
                                perblock/2, perblock/2, QBrush(QColor("#f1e2f3"))); 
            } else {
                painter.fillRect(start_x + map->getDfsX() * perblock, strat_y + map->getDfsY() * perblock, 
                                perblock, perblock, QBrush(Qt::darkMagenta));
            }
        }
        
        
    }
}


bool mazeWidget::isWalkable(int value) {
    // 检查一个单元格是否可行走
    return (value == 3 || value == 4 || value == 5 || 
            value == 6 || value == 7 || value == 8 || value == 9 || value == 10);
}

void mazeWidget::keyPressEvent(QKeyEvent* event) {
    if(!keybord_switch) return;
    int x = map->p_x;
    int y = map->p_y;
    //键盘移动逻辑：
    if(event->key() == Qt::Key_I || event->key() == Qt::Key_W) {
        if(isWalkable((*map)[x][y - 1])) {
            map->p_y--;
        }
    } else if(event->key() == Qt::Key_K || event->key() == Qt::Key_S) {
        if(isWalkable((*map)[x][y + 1])) {
            map->p_y++;
        }
    } else if(event->key() == Qt::Key_J || event->key() == Qt::Key_A) {
        if(isWalkable((*map)[x - 1][y])) {
            map->p_x--;
        }
    } else if(event->key() == Qt::Key_L || event->key() == Qt::Key_D) {
        if(isWalkable((*map)[x + 1][y])) {
            map->p_x++;
        }
    }
    
    // 标记玩家走过的路径
    if((*map)[map->p_x][map->p_y] != 5 && (*map)[map->p_x][map->p_y] != 6)
        (*map)[map->p_x][map->p_y] = 7;
    
    repaint();
    //到达终点
    // 竞赛模式下，检查是否到达终点
    if(competeMode && (*map)[map->p_x][map->p_y] == 6) {
        // 玩家到达终点
        map->winner = maze::Player;
        onCompetitionOver(static_cast<int>(maze::Player));
        return; // 结束键盘处理
    }
    // 普通模式下，检查是否到达终点
    else if((*map)[map->p_x][map->p_y] == 6) {
        map->makemap();
        repaint();
        grade += pow(map->getlevel(), 2);
        ui->grade_value->setText(QString::number(grade));
    }
}
void mazeWidget::time_update() {
    if(time != 0) {
        //计时中
        time--;
        ui->time_value->setText(QString::number(time));
        ui->progressBar->setValue(time / 2);
    } else {
        timer->stop();                          //停止计时器
        ui->progressBar->setVisible(false);     //隐藏进度条
        keybord_switch = false;                 //设置键盘响应
        painting_switch = false;                //绘图响应
        timing_switch = false;                  //计时响应为关闭状态
        repaint();                              //清除画布
        ui->start_btn->setEnabled(true);        
        ui->time_value->setText(" ");           
        ui->grade_value->setText(" ");         
        ui->stop_ptn->setEnabled(false);        //|设置各按钮与标签状态
        ui->end_btn->setEnabled(false);         
        ui->setting_btn->setEnabled(true);      
        //提示
        QMessageBox outgrade(QMessageBox::NoIcon,"", "得分:" + QString::number(grade), QMessageBox::Ok);
        outgrade.exec();
        //分数重置
        grade = 0;
    }
}
void mazeWidget::on_start_btn_clicked() {
    ui->label->setVisible(false);
    ui->solve_btn->setEnabled(true);
    painting_switch = true;
    timing_switch = true;
    keybord_switch = true;
    time = 200;
    timer->start(1000);
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(100);
    repaint();
    ui->time_value->setText(QString::number(time));
    ui->grade_value->setText(QString::number(grade));
    ui->start_btn->setEnabled(false);
    ui->stop_ptn->setEnabled(true);
    ui->end_btn->setEnabled(true);
    ui->setting_btn->setEnabled(false);
    ui->plaque_time->setText("时间");
    ui->plaque_grade->setText("分数");
}
void mazeWidget::on_stop_ptn_clicked() {
    if(stop_switch) {
        timing_switch = false;
        keybord_switch = false;
        timer->stop();
        ui->stop_ptn->setText("继续");
        stop_switch = false;
        //设置暂停状态
        if(map) {
            map->setPause(true);
        }

    } else {
        timing_switch = true;
        keybord_switch = true;
        timer->start();
        ui->stop_ptn->setText("暂停");
        stop_switch = true;

        // 取消暂停状态
        if(map) {
            map->setPause(false);
        }
    }
}
void mazeWidget::on_end_btn_clicked() {
    //设置终止状态
    if(map) {
        map->setExit(true);
    }

    timing_switch = false;
    painting_switch = false;
    keybord_switch = false;
    stop_switch = false;
    timer->stop();
    time = 0;
    grade = 0;
    ui->plaque_time->setText("  ");
    ui->plaque_grade->setText("  ");
    ui->progressBar->setVisible(false);
    ui->grade_value->setText(" ");
    ui->time_value->setText(" ");
    ui->stop_ptn->setText("暂停");
    ui->stop_ptn->setEnabled(false);
    ui->end_btn->setEnabled(false);
    ui->start_btn->setEnabled(true);
    ui->setting_btn->setEnabled(true);
    map->rebuildmap();
    ui->label->setVisible(true);
    ui->solve_btn->setEnabled(false);
    repaint();
}
void mazeWidget::on_rule_btn_clicked() {
    QMessageBox rule(QMessageBox::NoIcon, "规则", "计时200秒，根据迷宫等级与经过关卡记分。\n操作方式：WASD。", QMessageBox::Ok);
    rule.exec();
}
void mazeWidget::on_setting_btn_clicked() {
    QStringList difficultys;
    difficultys << tr("5阶迷宫") << tr("简单难度(10阶迷宫)") << tr("普通难度(20阶迷宫)") << tr("困难难度(40阶迷宫)");
    QString difficulty = QInputDialog::getItem(this, tr("选择难度"),
                                               tr("请选择一个条目"), difficultys, 0, false);
    if(difficulty == tr("5阶迷宫")) {
        delete map;
        map = new maze(5);
        map->makemap();
    } else if(difficulty == tr("简单难度(10阶迷宫)")) {
        delete map;
        map = new maze(10);
        map->makemap();
    } else if(difficulty == tr("普通难度(20阶迷宫)")) {
        delete map;
        map = new maze(20);
        map->makemap();
    } else if(difficulty == tr("困难难度(40阶迷宫)")) {
        delete map;
        map = new maze(40);
        map->makemap();
    }
}




void mazeWidget::on_solve_btn_clicked() {
    ui->solve_btn->setEnabled(false);

    // 连接信号槽（确保每次只连接一次）
    disconnect(map, &maze::mazeUpdated, this, &mazeWidget::maze_repaint);
    disconnect(map, &maze::searchOver, this, &mazeWidget::on_searchOver);
    connect(map, &maze::mazeUpdated, this, &mazeWidget::maze_repaint);
    connect(map, &maze::searchOver, this, &mazeWidget::on_searchOver);

    // 设置较低的可视化延迟，以便用户能看清过程
    map->setspeed(250);  // 100毫秒延迟

    map->solve();
   
}

// 添加新的槽函数处理搜索完成
void mazeWidget::on_searchOver() {
    // 等待2秒让用户查看结果
    QTimer::singleShot(2000, this, [this]() {
        // 重新生成地图并重置
        map->makemap();
        repaint();
        grade += pow(map->getlevel(), 2);
        ui->grade_value->setText(QString::number(grade));
        ui->solve_btn->setEnabled(true);
        ui->dfs_btn->setEnabled(true);
        ui->bfs_btn->setEnabled(true); //
    });
}

//更新迷宫显示
void mazeWidget::maze_repaint(){
    repaint();
}


void mazeWidget::on_dfs_btn_clicked()
{
    ui->dfs_btn->setEnabled(false);
    
    disconnect(map, &maze::mazeUpdated, this, &mazeWidget::maze_repaint);
    disconnect(map, &maze::searchOver, this, &mazeWidget::on_searchOver);
    connect(map, &maze::mazeUpdated, this, &mazeWidget::maze_repaint);
    connect(map, &maze::searchOver, this, &mazeWidget::on_searchOver);
    
    map->resetState(); // 重置状态

    // 设置可视化延迟
    map->setspeed(20); // 设置较慢的速度以便观察
    
    // 使用链栈实现的DFS搜索
    map->dfs_stack();
}

//BFS按钮点击的槽函数
void mazeWidget::on_bfs_btn_clicked()
{
    ui->bfs_btn->setEnabled(false);
    
    disconnect(map, &maze::mazeUpdated, this, &mazeWidget::maze_repaint);
    disconnect(map, &maze::searchOver, this, &mazeWidget::on_searchOver);
    connect(map, &maze::mazeUpdated, this, &mazeWidget::maze_repaint);
    connect(map, &maze::searchOver, this, &mazeWidget::on_searchOver);
    
    map->resetState();
    map->setspeed(20);
    // 使用链队列实现的BFS搜索
    map->bfs_queue();
}

bool mazeWidget::competitionStepUpdate_resetFirstCheck() {
    static bool firstCheck = true;
    bool result = firstCheck;
    firstCheck = true;
    return result;
}

// 实现人机竞速按钮的槽函数
void mazeWidget::on_compete_button_clicked() {
    if(!competeMode) {

        static_cast<void>(competitionStepUpdate_resetFirstCheck());

        // 开始竞赛
        competeMode = true;
        ui->compete_button->setText("停止竞赛");
        
        // 确保UI状态正确
        painting_switch = true; // 启用绘图
        keybord_switch = true;  // 启用键盘
        
        // 设置迷宫为竞赛模式
        map->setCompeteMode(true);
        // 更新界面
        repaint();

        map->dfsRunning = true;

        QTimer::singleShot(1000, this, [this]() {
        // 启动BFS
        map->bfsRunning = true;
        }); 


        
        // 启动定时器进行更新，设置较慢的速度(500ms)使移动可见
        competeTimer->start(500);
    } else {
        // 停止竞赛
        competeMode = false;
        ui->compete_button->setText("开始竞赛");
        
        map->dfsRunning = false;
        map->bfsRunning = false;
        competeTimer->stop();
        
        // 重置地图
        map->makemap();
        repaint();
    }
}


void mazeWidget::competitionStepUpdate() {

    // 每次调用时，让DFS和BFS各移动一步
    bool dfsUpdated = false;
    bool bfsUpdated = false;
    
    if(map->dfsRunning) {
        dfsUpdated = map->moveDfsOneStep();
    }
    
    if(map->bfsRunning) {
        bfsUpdated = map->moveBfsOneStep();
    }
    
    // if(!map->dfsRunning && !map->bfsRunning) {
    //     //QMessageBox::information(this, "竞赛提示", "算法无法到达终点，竞赛结束！");
    //     competeTimer->stop();
    //     competeMode = false;
    //     ui->compete_button->setText("开始竞赛");
        
    //     // 重置地图
    //     map->makemap();
    //     repaint();
    // }
    
    // 强制更新界面
    repaint();
}

// 处理竞赛结束
void mazeWidget::onCompetitionOver(int winner) {
    // 停止所有竞赛相关活动
    competeTimer->stop();
    map->dfsRunning = false;
    map->bfsRunning = false;
    
    QString winnerText;
    int scoreMultiplier = 0;
    
    if(winner == maze::Player) {
        winnerText = "恭喜您赢得了比赛！";
        scoreMultiplier = 1; 
    } else if(winner == maze::DFS) {
        winnerText = "DFS赢得了比赛！";
        scoreMultiplier = 1; 
    } else if(winner == maze::BFS) {
        winnerText = "BFS赢得了比赛！";
        scoreMultiplier = 1; 
    }
    
    // 计算并更新分数
    int earnedPoints = pow(map->getlevel(), scoreMultiplier);
    grade += earnedPoints;
    ui->grade_value->setText(QString::number(grade));
    
    // 显示结果
    QString message = winnerText + "\n您获得了 " + QString::number(earnedPoints) + " 分！";
    QMessageBox::information(this, "竞赛结束", message);
    
    // 保存分数
    if(grade > 0) {
        bool ok;
        QString playerName = QInputDialog::getText(this, "保存得分", 
                                                "请输入您的名字:", QLineEdit::Normal, 
                                                "", &ok);
        if(ok && !playerName.isEmpty()) {
            rank::addScore(playerName, grade);
            QMessageBox::information(this, "分数保存", "您的分数已保存到排行榜！");
        }
    }
    
    // 重置竞赛模式
    competeMode = false;
    ui->compete_button->setText("开始竞赛");
    
    // 重新生成地图
    QTimer::singleShot(500, this, [this]() {
        map->makemap();
        repaint();
    });
}

void mazeWidget::on_menu_btn_clicked()
{
    timer->stop();
    competeTimer->stop();
    
    if(map) {
        map->setExit(true);
    }
    
    timing_switch = false;
    painting_switch = false;
    keybord_switch = false;
    stop_switch = false;
    competeMode = false;
    
    if(grade > 0) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "保存分数", 
                                         "是否要保存您的得分？", 
                                         QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            bool ok;
            QString playerName = QInputDialog::getText(this, "保存得分", 
                                                  "请输入您的名字:", QLineEdit::Normal, 
                                                  "", &ok);
            if(ok && !playerName.isEmpty()) {
                rank::addScore(playerName, grade);
                QMessageBox::information(this, "分数保存", "您的分数已保存到排行榜！");
            }
        }
    }
    
    emit backToMenu();
    this->close();
}

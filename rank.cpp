#include "rank.h"
#include "ui_rank.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateTime>
#include <QMessageBox>
#include <algorithm>

// 定义静态常量
const QString rank::RANKING_FILE = "ranking.json";

rank::rank(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::rank)
{
    ui->setupUi(this);
    setWindowTitle("排行榜");
}

rank::~rank()
{
    delete ui;
}

void rank::showRanking() {
    // 加载排行榜数据
    QList<RankItem> rankings = loadRankings();
    
    // 创建表格并显示
    QTableWidget* table = createRankingTable(rankings);
    
    // 设置布局
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(table);
    
    setLayout(layout);
}

QTableWidget* rank::createRankingTable(const QList<RankItem>& rankings) {
    // 创建表格
    QTableWidget* table = new QTableWidget(this);
    table->setColumnCount(3);
    table->setRowCount(rankings.size());
    table->setHorizontalHeaderLabels({"玩家", "分数", "日期"});
    
    // 填充数据
    for(int i = 0; i < rankings.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(rankings[i].playerName));
        table->setItem(i, 1, new QTableWidgetItem(QString::number(rankings[i].score)));
        table->setItem(i, 2, new QTableWidgetItem(rankings[i].date));
    }
    
    // 调整列宽
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    return table;
}

QList<RankItem> rank::loadRankings() {
    QList<RankItem> rankings;
    
    // 从文件读取排行榜数据
    QFile file(RANKING_FILE);
    if(file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonArray array = doc.array();
        
        for(const QJsonValue& value : array) {
            QJsonObject obj = value.toObject();
            RankItem item;
            item.playerName = obj["name"].toString();
            item.score = obj["score"].toInt();
            item.date = obj["date"].toString();
            rankings.append(item);
        }
        
        file.close();
    }
    
    // 排序
    std::sort(rankings.begin(), rankings.end());
    
    return rankings;
}

bool rank::saveRankings(const QList<RankItem>& rankings) {
    // 创建JSON数组
    QJsonArray newArray;
    for(const RankItem& item : rankings) {
        QJsonObject obj;
        obj["name"] = item.playerName;
        obj["score"] = item.score;
        obj["date"] = item.date;
        newArray.append(obj);
    }
    
    // 保存到文件
    QJsonDocument doc(newArray);
    QFile file(RANKING_FILE);
    if(file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        return true;
    }
    
    return false;
}

bool rank::addScore(const QString& playerName, int score) {
    if(score <= 0 || playerName.isEmpty()) return false;
    
    // 读取现有排行榜
    QList<RankItem> rankings = loadRankings();
    
    // 添加新记录
    RankItem newItem;
    newItem.playerName = playerName;
    newItem.score = score;
    newItem.date = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
    rankings.append(newItem);
    
    // 排序
    std::sort(rankings.begin(), rankings.end());
    
    // 只保留前10名
    if(rankings.size() > 10) {
        rankings = rankings.mid(0, 10);
    }
    
    // 保存回文件
    return saveRankings(rankings);
}

bool rank::clearRankings() {
    QFile file(RANKING_FILE);
    if(file.open(QIODevice::WriteOnly)) {
        QJsonArray emptyArray;
        QJsonDocument doc(emptyArray);
        file.write(doc.toJson());
        file.close();
        return true;
    }
    
    return false;
}
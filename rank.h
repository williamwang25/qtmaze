#ifndef RANK_H
#define RANK_H

#include <QWidget>
#include <QList>
#include <QString>
#include <QDateTime>
#include <QTableWidget>
#include <QJsonArray>

// 排行榜条目结构体
struct RankItem {
    QString playerName;
    int score;
    QString date;
    
    bool operator<(const RankItem& other) const {
        return score > other.score; // 降序排列
    }
};

namespace Ui {
class rank;
}

class rank : public QWidget
{
    Q_OBJECT

public:
    explicit rank(QWidget *parent = nullptr);
    ~rank();
    
    // 显示排行榜
    void showRanking();
    
    // 添加新的排名记录
    static bool addScore(const QString& playerName, int score);
    
    // 从文件加载排行榜数据
    static QList<RankItem> loadRankings();
    
    // 保存排行榜数据到文件
    static bool saveRankings(const QList<RankItem>& rankings);
    
    // 清空排行榜
    static bool clearRankings();

private:
    Ui::rank *ui;
    
    // 创建排行榜表格
    QTableWidget* createRankingTable(const QList<RankItem>& rankings);
    
    // 排行榜文件路径
    static const QString RANKING_FILE;
};

#endif // RANK_H
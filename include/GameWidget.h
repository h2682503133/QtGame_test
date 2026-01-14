#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QKeyEvent>
#include <vector>
#include <algorithm>

#include "GameObject.h"
#include "EnemyBase.h"
#include "EnemyNormal.h"
#include <Player.h>
#include "ui_Form.h"
class GameWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GameWidget(QWidget *parent = nullptr);
    ~GameWidget() override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
    void gameUpdate();
    void spawnEnemy();

private:
    Ui::Form *ui;
    // 按键长按状态标记
    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;
    // 游戏核心状态
    Player* m_player; //自机对象
    int score;
    bool gameOver=false;
    bool gamePause=false;
    // 玩家移动速度(预留，对接玩家类使用)
    int playerSpeed;
    // 定时器对象
    QTimer *gameTimer;
    QTimer *enemyTimer;
    //按钮点击的槽函数声明
    void onBtnStartClicked();
    void onBtnRestartClicked();

    std::vector<EnemyBase*> m_activeEnemies;  // 屏幕中活跃的敌机
    std::vector<EnemyBase*> m_enemyPool;      // 敌机对象池-存放闲置敌机，复用无内存碎片

    EnemyBase* getEnemyFromPool();            // 从池中取出敌机
    void recycleEnemy(EnemyBase* enemy);      // 回收敌机到池中复用

    void updateAllEnemies();                  // 更新所有敌机的移动+出界判定
    void drawAllEnemies(QPainter& painter);   // 绘制所有敌机
    void checkPlayerEnemyCollision();         // 检测玩家与敌机碰撞

    //敌机生成相关
    std::vector<EnemyTypeItem> m_enemyTypePool;  // 敌机类型池：类型+创建函数
    std::vector<int> m_weightList;               // 自动读取的权重列表，和类型池一一对应
    int m_totalWeight = 0;                       // 总权重
};

#endif // GAMEWIDGET_H

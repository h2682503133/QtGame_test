#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H
#include <QWidget>
#include <QTimer>
#include <Player.h>
#include "ui_gamewidget.h"
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
    Ui::gamewidget *ui;
    // 按键长按状态标记
    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;
    // 游戏核心状态
    Player* m_player; //自机对象
    int score;
    bool gameOver;
    // 玩家移动速度(预留，对接玩家类使用)
    int playerSpeed;
    // 定时器对象
    QTimer *gameTimer;
    QTimer *enemyTimer;
};

#endif // GAMEWIDGET_H

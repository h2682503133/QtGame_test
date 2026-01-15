#ifndef PLAYER_H
#define PLAYER_H
#include <QTime>
#include "GameObject.h"

// 自机类 - 玩家控制的飞机，公有继承GameObject通用基类
class Player : public GameObject
{
    Q_OBJECT
public:
    // 构造函数：默认出生坐标x/y、自机贴图尺寸、圆形碰撞半径、移动速度
    // 自机默认参数：宽40 高50、碰撞半径18(比贴图小，贴合飞机机身，不是全屏碰撞)、移动速度8
    Player(int x = 0, int y = 0);
    ~Player() override = default;

    //空置移动使用下方
    void move() override;

    //自机移动+碰撞检测
    void moveUp();    // 向上移动
    void moveDown(int windowHeight); // 向下移动 + 屏幕下边界限制
    void moveLeft();  // 向左移动 + 屏幕左边界限制
    void moveRight(int windowWidth); // 向右移动 + 屏幕右边界限制
    //发射子弹
    void shootBullet();
    //无敌状态（用于可能拓展无敌道具或调试模式）
    void setInvincible(bool invincible);
    bool isInvincible() const;           
    void OnDead();
private:
    bool m_isInvincible; // 自机无敌标记(额外特性，区别于通用防御系数)

    int m_shootInterval; // 发射间隔单位毫秒
    QTime m_lastShootTime; 
    int counter=0;  //计数器 用于子弹的轮换发射
signals:  //敌机死亡信号，携带加分值
    void dead();
};

#endif // PLAYER_H
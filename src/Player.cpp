#include "Player.h"
#include "PlayerBullet.h"
#include "GameWidget.h"
QPixmap Player::s_img;
// 自机构造函数实现
Player::Player(int x, int y)
    : GameObject(x, y, 40, 50, 18, 8), // 父类传参：坐标x/y、贴图宽40高50、碰撞半径18、移动速度8
      m_isInvincible(false)
{
    // 自机默认配置 - 玩家专属，初始化时自动绑定
    setCamp(2);       // 阵营：玩家阵营(2)
    setHp(3);                   // 初始血量3条命
    setMaxHp(3);                // 最大血量3条命
    setDamage(1);               // 子弹伤害1
    setDefense(0.0f);           // 默认无防御，可通过setInvincible(true)开启无敌

    m_shootInterval = 300; // 300毫秒一发
    m_lastShootTime = QTime::currentTime();
}
void Player::loadPlayerResource()
{
    s_img=this->loadImgFromFile("player", 50, 50);
    this->m_img=&s_img;
}
// 重写父类纯虚move函数(空实现)，自机移动由按键调用专属接口控制
void Player::move()
{
}

void Player::shootBullet()
{
    if(!this->isAlive()) return; // 玩家死亡不发射子弹
    QTime currentTime = QTime::currentTime();
    // 计算时间差
    int elapsed = m_lastShootTime.msecsTo(currentTime);
    // 时间差小于间隔直接返回
    if (elapsed < m_shootInterval)
    {
        return;
    }
    switch(counter)
    {
        case 0: 
        {
            // 出生位置：玩家贴图的正上方中心，Y轴-10
            PlayerBullet1* bullet = new PlayerBullet1(this->getImgRect().center().x(), 
                                                    this->getImgRect().top() - 10,
                                                    this->parent());
            bullet->initPlayerBullet(-90.0);
            
        }
        case 1: 
        {
        // 出生位置：玩家贴图的正上方中心，Y轴-15 x轴左右10
            PlayerBullet1* bullet = new PlayerBullet1(this->getImgRect().center().x()-10, 
                                                    this->getImgRect().top() - 15,
                                                    this->parent());
            bullet->initPlayerBullet(-90.0);
            bullet = new PlayerBullet1(this->getImgRect().center().x()+10, 
                                                    this->getImgRect().top() - 15,
                                                    this->parent());
            bullet->initPlayerBullet(-90.0);
            m_lastShootTime = currentTime;
        }
        case 2:
        {
            // 出生位置：玩家贴图的正上方中心，Y轴-20
            PlayerBullet2* bullet = new PlayerBullet2(this->getImgRect().center().x(), 
                                                    this->getImgRect().top() - 20,
                                                    this->parent());
            bullet->initPlayerBullet(-90.0);
            
        }
        //轮空一轮 置零计时器
        default:
            counter = 0;
            break;
    }
    counter+=1;
    m_lastShootTime = currentTime;
}
// 向上移动 - 无边界上限(飞机可以飞出屏幕上方)
void Player::moveUp()
{
    moveOffset(0, -getSpeed());
}

// 向下移动 + 屏幕下边界检测：飞机底部不超出窗口高度
void Player::moveDown(int windowHeight)
{
    if (getImgRect().bottom() < windowHeight)
    {
        moveOffset(0, getSpeed());
    }
}

// 向左移动 + 屏幕左边界检测：飞机左侧不超出x=0
void Player::moveLeft()
{
    if (getImgRect().left() > 0)
    {
        moveOffset(-getSpeed(), 0);
    }
}

// 向右移动 + 屏幕右边界检测：飞机右侧不超出窗口宽度
void Player::moveRight(int windowWidth)
{
    if (getImgRect().right() < windowWidth)
    {
        moveOffset(getSpeed(), 0);
    }
}

// 设置无敌状态：本质是修改通用基类的防御系数(1.0=完全无敌，伤害乘0)
void Player::setInvincible(bool invincible)
{
    m_isInvincible = invincible;
    setDefense(invincible ? 1.0f : 0.0f);
}

// 获取无敌状态
bool Player::isInvincible() const
{
    return m_isInvincible;
}
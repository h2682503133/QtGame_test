#include "BulletBase.h"
#include <cmath>

BulletBase::BulletBase(QObject *parent) 
{
    this->setParent(parent);
    this->setAlive(false); // 初始为非存活状态
}

BulletBase::BulletBase(int x, int y, int imgWidth, int imgHeight, int collideRadius, int speed)
    : GameObject(x, y, imgWidth, imgHeight, collideRadius, speed)
{
    m_shootAngle = 0.0;
    this->setAlive(false);
}

// 子弹初始化
void BulletBase::initBullet(int camp, double shootAngle, int damage)
{
    this->setCamp(camp);       // 设置阵营（玩家子弹=2，敌机子弹=3）
    this->setShootAngle(shootAngle); // 设置发射角度
    this->setDamage(damage);   // 设置子弹伤害（复用父类属性）
    this->setAlive(true);      // 标记子弹存活（复用父类属性）
    this->setHp(1);            // 子弹血量为1，击中即销毁
    this->setMaxHp(1);
    this->setDefense(0.0);     // 子弹无防御
}
//默认使用直线运动
void BulletBase::move()
{
    if(!this->isAlive()) return; // 非存活子弹不移动

    double radian = m_shootAngle * M_PI / 180.0;
    int dx = static_cast<int>(cos(radian) * this->getSpeed());
    int dy = static_cast<int>(sin(radian) * this->getSpeed());
    this->moveOffset(dx, dy);
}

// 检测子弹出界：任意方向超出窗口范围均判定为出界
bool BulletBase::isOutOfWindow(int winWidth, int winHeight)
{
    QRect bulletRect = this->getImgRect();
    return bulletRect.right() < 0 || bulletRect.left() > winWidth ||
           bulletRect.bottom() < 0 || bulletRect.top() > winHeight;
}

// Get/Set 发射角度
double BulletBase::getShootAngle() const
{
    return m_shootAngle;
}
void BulletBase::setShootAngle(double newShootAngle)
{
    m_shootAngle = newShootAngle;
}

#include "PlayerBullet2.h"

// 无参构造
PlayerBullet2::PlayerBullet2(QObject *parent) : BulletBase(parent)
{
}

PlayerBullet2::PlayerBullet2(int x, int y, QObject *parent) 
    : BulletBase(x, y, 14, 14, 4, 8) // 固定值宽14、高14、碰撞半径4、速度8
{
    this->m_imgWidth = 14;
    this->m_imgHeight = 14;
    this->setParent(parent);
    this->loadImgFromFile("playerbullet_2", this->m_imgWidth, this->m_imgHeight);
    //玩家子弹的阵营为2
    this->setCamp(2);
    //子弹血量为1，击中即销毁
    this->setHp(1);
    this->setMaxHp(1);
    //该子弹穿透防御为1
    this->setDefense(1.0);
    
}

void PlayerBullet2::initPlayerBullet(double shootAngle)
{
    this->initBullet(this->getCamp(), shootAngle, 2); // 伤害值为2
    // 固定子弹为存活状态
    this->setAlive(true);
    //同步碰撞盒到子弹贴图中心
    this->setCollideCenter(this->getImgRect().center().x(), this->getImgRect().center().y());
    this->setReady(true);
}
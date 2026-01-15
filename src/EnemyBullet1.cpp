
#include "EnemyBullet1.h"

// 无参构造
EnemyBullet1::EnemyBullet1(QObject *parent) : BulletBase(parent)
{
}

EnemyBullet1::EnemyBullet1(int x, int y, QObject *parent) 
    : BulletBase(x, y, 6, 14, 2, 8) // 固定值宽6、高14、碰撞半径2、速度8
{
    this->m_imgWidth = 6;
    this->m_imgHeight = 14;
    this->setParent(parent);
    this->loadImgFromFile("enemybullet_1", this->m_imgWidth, this->m_imgHeight);
    //敌人子弹的阵营为3
    this->setCamp(3);
    //子弹血量为1，击中即销毁
    this->setHp(1);
    this->setMaxHp(1);
    //子弹无防御
    this->setDefense(0.0);
    
}

void EnemyBullet1::initEnemyBullet(double shootAngle)
{
    this->initBullet(this->getCamp(), shootAngle, 1); // 伤害值为1
    // 固定子弹为存活状态
    this->setAlive(true);
    //同步碰撞盒到子弹贴图中心
    this->setCollideCenter(this->getImgRect().center().x(), this->getImgRect().center().y());
    this->setReady(true);
}
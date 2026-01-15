#include "EnemyStatic.h"
#include "EnemyBullet.h"
EnemyStatic::EnemyStatic(int winWidth,QObject *parent) : EnemyBase(winWidth)
{
    this->setWeight(4);
    this->setReady(true);
    this->setImgSize(50, 50);
    this->setCollideRadius(25);
    this->maxHight = 100;
    this->setMaxHp(10);
    this->setSpeed(1);
    this->setScoreReward(25);

    m_shootInterval = 1500; // 1500毫秒一发
    m_lastShootTime = QTime::currentTime();
}
//贴图加载函数
void EnemyStatic::loadEnemyResource()
{
    
    this->loadImgFromFile("enemy_static", this->m_imgWidth, this->m_imgHeight);
}
void EnemyStatic::move()
{
    if (this->isAlive() && !m_isOutOfWindow && getImgRect().top()<maxHight)
    {
        this->moveOffset(0, m_enemySpeed);
    }
}
void EnemyStatic::shootBullet()
{
    
    if(!this->isAlive()) return; // 死亡不发射子弹
    QTime currentTime = QTime::currentTime();
    // 计算时间差
    int elapsed = m_lastShootTime.msecsTo(currentTime);
    // 时间差小于间隔直接返回
    if (elapsed < m_shootInterval)
    {
        return;
    }
    // 出生位置：贴图的正下方中心，Y轴+10
    EnemyBullet1* bullet = new EnemyBullet1(this->getImgRect().center().x(), 
                                            this->getImgRect().bottom() + 10,
                                            this->parent());
    bullet->initEnemyBullet(90.0);
    m_lastShootTime = currentTime;
    
}
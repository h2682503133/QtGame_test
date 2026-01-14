#include "EnemyBase.h"
#include <QRandomGenerator>

EnemyBase::EnemyBase(int winWidth)
    : GameObject(0, 0, 40, 40, 18),  
      m_scoreReward(10),             
      m_enemySpeed(3),               
      m_isOutOfWindow(false),         
      m_imgWidth(40),                
      m_imgHeight(40),               
      m_collideRadius(18)             
{
    this->setCamp(3);

   
    this->setMaxHp(1);        // 敌机血量默认1，一碰就死
    this->setHp(1);           // 初始满血
    this->setDamage(100);     // 碰撞玩家秒杀（玩家阵营2，触碰即扣血）
    this->setDefense(0.0f);   // 敌机无防御，全额吃伤害

    //敌机生成规则 - 窗口顶部随机X坐标生成，保证敌机在屏幕内
    int randomX = QRandomGenerator::global()->bounded(0, m_winWidth - m_imgWidth);
    this->setImgPos(randomX, 0);  // 设置贴图坐标：顶部随机X，Y=0
    this->setCollideCenter(randomX + m_imgWidth/2, m_imgHeight/2); // 碰撞圆心与贴图居中对齐
}


void EnemyBase::move()
{
    if (this->isAlive() && !m_isOutOfWindow)
    {
        //自动同步 调用GameObject的moveOffset
        this->moveOffset(0, m_enemySpeed);
    }
}

//出界检测
void EnemyBase::checkOutOfWindow(int winHeight)
{
    if (this->getImgRect().top() > winHeight)
    {
        m_isOutOfWindow = true;
        this->setAlive(false); // 出界即死亡，等待主界面清理
    }
}

void EnemyBase::onEnemyDead()
{
    // 父类仅标记死亡，无其他逻辑
    this->setAlive(false);
}

void EnemyBase::loadEnemyResource()
{
}

int EnemyBase::getScoreReward() const
{
    return m_scoreReward;
}

void EnemyBase::setScoreReward(int scoreReward)
{
    m_scoreReward = scoreReward;
}

void EnemyBase::setEnemySpeed(int speed)
{
    m_enemySpeed = speed;
}

int EnemyBase::getEnemySpeed() const
{
    return m_enemySpeed;
}

bool EnemyBase::isEnemyOutOfWindow() const
{
    return m_isOutOfWindow;
}

int EnemyBase::getWeight() const { return m_weight; }
void EnemyBase::setWeight(int weight) { m_weight = weight; }
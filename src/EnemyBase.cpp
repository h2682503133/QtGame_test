#include "EnemyBase.h"
#include "Player.h"
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
    this->setHp(0);           // 初始满血
    this->setDamage(100);     // 碰撞玩家秒杀（玩家阵营2，触碰即扣血）
    this->setDefense(0.0f);   // 敌机无防御，全额吃伤害
}


void EnemyBase::move()
{
    if (this->isAlive() && !m_isOutOfWindow)
    {
        this->moveOffset(0, m_enemySpeed);
    }
}
bool EnemyBase::checkAllEnemyCollideWithPlayer(Player* player, bool& gameOver)
{
    if (isAlive())
    {
        // 调用GameObject父类的圆形碰撞检测函数
        if (player->isCircleCollide(*this))
        {
            player->takeDamage(getDamage());  // 玩家扣血
            gameOver = true;                         // 修改游戏结束标记（引用传参）
            return true;                             // 碰撞触发，直接返回
        }
    }
    return false;
}
//出界检测
void EnemyBase::checkOutOfWindow(int winHeight)
{
    if (this->getImgRect().top() > winHeight)
    {
        m_isOutOfWindow = true;
        this->setAlive(false); // 出界即死亡，但该方式不调用死亡回调
    }
    else
    {
        m_isOutOfWindow = false;
    }
}

void EnemyBase::onDead()
{
    setAlive(false);
    emit enemyDead(this->getScoreReward());
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

EnemyType EnemyBase::getEnemyType() const { return m_enemyType; }
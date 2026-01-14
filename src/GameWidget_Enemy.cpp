#include "GameWidget.h"
#include "EnemyBase.h"

#include <QRandomGenerator>
#include <QMessageBox>
#include <QRandomGenerator>

void GameWidget::spawnEnemy()
{
    if(gameOver || m_enemyTypePool.empty() || m_totalWeight <=0) return;

    //权重随机抽取 命中对应的敌机类型项
    EnemyType curEnemyType = EnemyType::NormalEnemy;
    EnemyCreatorFunc curCreator = nullptr;
    int randNum = QRandomGenerator::global()->bounded(0, m_totalWeight);
    int weightSum = 0;
    int targetIndex = -1;

    // 遍历权重列表，命中区间，得到对应下标
    for (int i=0; i<m_weightList.size(); i++)
    {
        weightSum += m_weightList[i];
        if (randNum < weightSum)
        {
            targetIndex = i;
            break;
        }
    }

    // 命中成功，获取对应的敌机类型+创建函数
    if(targetIndex != -1 && targetIndex < m_enemyTypePool.size())
    {
        curEnemyType = m_enemyTypePool[targetIndex].type;
        curCreator = m_enemyTypePool[targetIndex].creator;
    }

    //从敌机池取机 / 自动创建敌机
    EnemyBase* enemy = getEnemyFromPool();
    if (enemy == nullptr && curCreator != nullptr)
    {
        enemy = curCreator(this->width());
        if(enemy) enemy->loadEnemyResource();
    }

    //敌机重置状态 加入活跃队列
    if(enemy)
    {
        enemy->setAlive(true);
        enemy->setHp(enemy->getMaxHp());
        int randomX = QRandomGenerator::global()->bounded(0, this->width() - enemy->getImgRect().width());
        enemy->setImgPos(randomX, 0);
        enemy->setCollideCenter(randomX + enemy->getImgRect().width()/2, enemy->getImgRect().height()/2);
        m_activeEnemies.push_back(enemy);
    }
}

//从敌机池获取闲置敌机
EnemyBase* GameWidget::getEnemyFromPool()
{
    if (!m_enemyPool.empty())
    {
        EnemyBase* enemy = m_enemyPool.back();
        m_enemyPool.pop_back();
        return enemy;
    }
    return nullptr;
}

//回收敌机到对象池
void GameWidget::recycleEnemy(EnemyBase* enemy)
{
    enemy->setAlive(false);
    m_enemyPool.push_back(enemy);
    // 从活跃队列中移除该敌机
    auto it = std::find(m_activeEnemies.begin(), m_activeEnemies.end(), enemy);
    if (it != m_activeEnemies.end())
    {
        m_activeEnemies.erase(it);
    }
}

//更新所有敌机的状态：移动 + 出界检测
void GameWidget::updateAllEnemies()
{
    for (EnemyBase* enemy : m_activeEnemies)
    {
        if (enemy->isAlive())
        {
            enemy->move();  // 调用敌机自身的移动逻辑：垂直匀速下落
            enemy->checkOutOfWindow(this->height());  // 检测是否飞出窗口底部

            // 出界则回收敌机到池子里
            if (enemy->isEnemyOutOfWindow())
            {
                recycleEnemy(enemy);
            }
        }
        else
        {
            // 敌机死亡则回收
            recycleEnemy(enemy);
            score += enemy->getScoreReward();  //击落敌机加分
        }
    }
}

//绘制所有敌机
void GameWidget::drawAllEnemies(QPainter& painter)
{
    for (EnemyBase* enemy : m_activeEnemies)
    {
        if (enemy->isAlive())
        {
            painter.drawPixmap(enemy->getImgRect(), enemy->getPixmap());
        }
    }
}

//玩家与敌机的碰撞检测
void GameWidget::checkPlayerEnemyCollision()
{
    if (!m_player->isAlive() || gameOver) return;

    for (EnemyBase* enemy : m_activeEnemies)
    {
        if (enemy->isAlive())
        {
            // 调用GameObject父类的圆形碰撞检测函数
            if (m_player->isCircleCollide(*enemy))
            {
                m_player->takeDamage(enemy->getDamage());  // 玩家扣血
                gameOver = true;                           // 游戏结束
                break;
            }
        }
    }
}
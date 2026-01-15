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
    int randNum = QRandomGenerator::global()->bounded(1, m_totalWeight);
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
        if(enemy) 
        {   
            QMetaObject::invokeMethod(enemy, &EnemyBase::loadEnemyResource, Qt::QueuedConnection);
            enemy->setParent(this);
        }
    }
    else if(enemy != nullptr)
    {
        
        enemy->disconnect();
        enemy->setParent(this);
    }
    //绑定信号槽
    
    if(enemy)
    {
        connect(enemy, &EnemyBase::enemyDead, this, &GameWidget::addEnemyScore);
    }
    
    //敌机重置状态 加入活跃队列
    if(enemy)
    {
        enemy->setAlive(true);
        enemy->setHp(enemy->getMaxHp());
        int randomX = QRandomGenerator::global()->bounded(0, this->width() - enemy->getImgRect().width());
        enemy->setImgPos(randomX, 0);
        enemy->setCollideCenter(randomX + enemy->getImgRect().width()/2, enemy->getImgRect().height()/2);
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
}

//更新所有敌机的状态：移动 + 出界检测
void GameWidget::updateAllEnemies()
{
    QList<EnemyBase*> enemyList = this->findChildren<EnemyBase*>(Qt::FindDirectChildrenOnly);
    for (EnemyBase* enemy : enemyList)
    {
        if (enemy->isAlive()&& enemy->isReady())
        {
            enemy->move();  // 调用敌机自身的移动逻辑：垂直匀速下落
            enemy->checkOutOfWindow(this->height());  // 检测是否飞出窗口底部

            // 出界则回收敌机到池子里
            if (enemy->isEnemyOutOfWindow())
            {
                recycleEnemy(enemy);
            }
        }
        else if(!enemy->isAlive()&&enemy->isReady())
        {
            // 敌机死亡则回收
            recycleEnemy(enemy);
        }
    }
}

//绘制所有敌机
void GameWidget::drawAllEnemies(QPainter& painter)
{
    QList<EnemyBase*> enemyList = this->findChildren<EnemyBase*>(Qt::FindDirectChildrenOnly);
    for (EnemyBase* enemy : enemyList)
    {
        if (enemy->isAlive()&& enemy->isReady())
        {
            painter.drawPixmap(enemy->getImgRect(), enemy->getPixmap());
        }
    }
}
void GameWidget::EnemiesShoot()
{
    QList<EnemyBase*> enemyList = this->findChildren<EnemyBase*>(Qt::FindDirectChildrenOnly);
    for (EnemyBase* enemy : enemyList)
    {
        if (enemy->isAlive() && enemy->isReady())
        {
            enemy->shootBullet();
        }
    }
}

//玩家与敌机的碰撞检测
void GameWidget::checkPlayerEnemyCollision()
{
    if (!m_player->isAlive() || gameOver) return;
    QList<EnemyBase*> enemyList = this->findChildren<EnemyBase*>(Qt::FindDirectChildrenOnly);
    for (EnemyBase* enemy : enemyList)
    {
        enemy->checkAllEnemyCollideWithPlayer(m_player,gameOver);
    }
}
//初始化敌人生成池
void GameWidget::initEnemyTypePool()
{
    m_enemyTypePool.clear();
    m_weightList.clear();
    m_totalWeight = 0;

    EnemyTypeItem item1;
    item1.type = EnemyType::NormalEnemy;
    item1.creator = EnemyNormal::Create;
    m_enemyTypePool.push_back(item1);

    EnemyTypeItem item2;
    item2.type = EnemyType::StaticEnemy;
    item2.creator = EnemyStatic::Create;
    m_enemyTypePool.push_back(item2);
    //遍历类型池累加总权重
    for (const auto& item : m_enemyTypePool)
    {
        // 创建临时敌机对象，只为读取权重
        EnemyBase* tempEnemy = item.creator(this->width());
        if (tempEnemy)
        {
            int enemyWeight = tempEnemy->getWeight(); // 自动读取子类自身的权重
            m_weightList.push_back(enemyWeight);      // 存入权重列表
            m_totalWeight += enemyWeight;             // 自动累加总权重
            tempEnemy->setParent(nullptr);  // 强制解除父对象（防止意外绑定）
            tempEnemy->deleteLater();       // 延时销毁
            tempEnemy = nullptr;
            QCoreApplication::processEvents();
        }
    }
}
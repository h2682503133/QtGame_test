#include "GameWidget.h"
#include "BulletBase.h"
#include "EnemyBase.h"
#include "Player.h"

void GameWidget::drawAllBullets(QPainter& painter)
{
    QList<BulletBase*> bulletList = this->findChildren<BulletBase*>(Qt::FindDirectChildrenOnly);
    for (int i = bulletList.size()-1; i >= 0; i--) // 倒序遍历
    {
        BulletBase* bullet = bulletList[i];
        if (bullet->isAlive())
        {
            painter.drawPixmap(bullet->getImgRect(), bullet->getPixmap());
        }
    }
}

void GameWidget::updateAllBullets()
{
    // 获取所有子弹对象
    QList<BulletBase*> allBullets = this->findChildren<BulletBase*>(Qt::FindDirectChildrenOnly);
    for (int i = allBullets.size()-1; i >= 0; i--) // 倒序遍历，防止删除后迭代器失效
    {
        BulletBase* bullet = allBullets[i];
        if(bullet->isAlive()&& bullet->isReady())
        {
            bullet->move();
            // 出界标记死亡直接销毁
            if(bullet->isOutOfWindow(this->width(), this->height()))
            {
                bullet->onDead();
            }
            else
            {
                // 非存活子弹直接销毁 兜底条件 预防设置死亡但未删除
                if(!bullet->isAlive())
                bullet->onDead();
            }
        }
    }
}

void GameWidget::checkBulletCollisions()
{
    if(gameOver || !m_player->isAlive()) return;

    QList<BulletBase*> allBullets = this->findChildren<BulletBase*>(Qt::FindDirectChildrenOnly);
    QList<EnemyBase*> allEnemys = this->findChildren<EnemyBase*>(Qt::FindDirectChildrenOnly);

    for (int i = allBullets.size()-1; i >= 0; i--)
    {
        BulletBase* bullet = allBullets[i];
        if(!bullet->isAlive()) continue;

        //击中敌机(阵营不是3)
        if(bullet->getCamp() != 3)
        {
            for (int j = allEnemys.size()-1; j >= 0; j--) // 倒序遍历，防止删除后迭代器失效
            {
                EnemyBase* enemy = allEnemys[j];
                if(enemy->isAlive() && bullet->isReady()&& !bullet->isSameCamp(enemy) && bullet->isCircleCollide(*enemy))
                {
                    enemy->takeDamage(bullet->getDamage()); // 敌机扣血
                    bullet->onDead();            // 子弹击中后死亡
                    break;
                }
            }
        }

        //子弹击中玩家(阵营不是2)
        else if(bullet->getCamp() != 2)
        {
            if(m_player->isAlive() && !bullet->isSameCamp(m_player) && bullet->isCircleCollide(*m_player))
            {
                m_player->takeDamage(bullet->getDamage()); // 玩家扣血
                bullet->setAlive(false);                    // 子弹击中后死亡
                if(!m_player->isAlive()) gameOver = true;  // 玩家死亡游戏结束
            }
        }
    }
}
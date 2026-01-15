#ifndef ENEMYSTATIC_H
#define ENEMYSTATIC_H

#include "EnemyBase.h"

// 基础敌机子类 使用父类EnemyBase所有默认值
class EnemyStatic : public EnemyBase
{
public:
    
    explicit EnemyStatic(int winWidth, QObject *parent);
    // 加载贴图，尺寸/碰撞半径等完全用父类默认值(50*50 贴图 + 25 碰撞半径)
    void loadEnemyResource() override;
    static EnemyBase* Create(int winWidth)
    {
        EnemyStatic* enemy = new EnemyStatic(winWidth, nullptr);
        return enemy;
    }
    void move() override;
    void shootBullet() override;
protected:
    int maxHight;
    EnemyType type = EnemyType::StaticEnemy;
    int m_shootInterval=1500; // 发射间隔单位毫秒
    QTime m_lastShootTime; 
};

#endif // ENEMYSTATIC_H
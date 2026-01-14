#ifndef ENEMYNORMAL_H
#define ENEMYNORMAL_H

#include "EnemyBase.h"

// 基础敌机子类 使用父类EnemyBase所有默认值
class EnemyNormal : public EnemyBase
{
public:
    explicit EnemyNormal(int winWidth, QObject *parent);
    // 加载贴图，尺寸/碰撞半径等完全用父类默认值(40*40 贴图 + 18 碰撞半径)
    void loadEnemyResource() override;
    static EnemyBase* Create(int winWidth)
    {
        EnemyNormal* enemy = new EnemyNormal(winWidth, nullptr);
        return enemy;
    }
};

#endif // ENEMYNORMAL_H
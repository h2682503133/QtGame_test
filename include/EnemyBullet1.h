#ifndef ENEMYBULLET1_H
#define ENEMYBULLET1_H
#include "BulletBase.h"
#include <QObject>

// 敌人子弹1号：继承自BulletBase，固定所有属性，仅位置、角度可变
class EnemyBullet1 : public BulletBase
{
    Q_OBJECT
public:
    explicit EnemyBullet1(QObject *parent = nullptr);
    explicit EnemyBullet1(int x, int y, QObject *parent = nullptr);
    void initEnemyBullet(double shootAngle);
};

#endif // ENEMYBULLET1_H
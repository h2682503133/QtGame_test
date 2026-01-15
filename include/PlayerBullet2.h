#ifndef PLAYERBULLET2_H
#define PLAYERBULLET2_H
#include "BulletBase.h"
#include <QObject>

// 玩家专属子弹2号：继承自BulletBase，固定所有属性，仅位置、角度可变
class PlayerBullet2 : public BulletBase
{
    Q_OBJECT
public:
    explicit PlayerBullet2(QObject *parent = nullptr);
    explicit PlayerBullet2(int x, int y, QObject *parent = nullptr);
    void initPlayerBullet(double shootAngle);
};

#endif // PLAYERBULLET2_H
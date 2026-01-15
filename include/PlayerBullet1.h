#ifndef PLAYERBULLET1_H
#define PLAYERBULLET1_H
#include "BulletBase.h"
#include <QObject>

// 玩家专属子弹1号：继承自BulletBase，固定所有属性，仅位置、角度可变
class PlayerBullet1 : public BulletBase
{
    Q_OBJECT
    public:
        explicit PlayerBullet1(QObject *parent = nullptr);
        explicit PlayerBullet1(int x, int y, QObject *parent = nullptr);
        void initPlayerBullet(double shootAngle);
    private:
        static QPixmap s_img;
};

#endif // PLAYERBULLET1_H
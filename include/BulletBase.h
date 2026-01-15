#ifndef BULLETBASE_H
#define BULLETBASE_H
#include "GameObject.h"
#include <QObject>

class BulletBase : public GameObject
{
    Q_OBJECT
public:
    // 复用父类的两个构造函数，完全一致，无新增
    explicit BulletBase(QObject *parent = nullptr);
    explicit BulletBase(int x, int y, int imgWidth, int imgHeight, int collideRadius, int speed = 8);

    // 子弹核心初始化
    void initBullet(int camp, double shootAngle, int damage = 1);

    // 检测子弹出界(上下左右超出窗口都算)
    bool isOutOfWindow(int winWidth, int winHeight);

    //发射角度
    double getShootAngle() const;
    void setShootAngle(double newShootAngle);

    // 子弹移动
    void move() override;

signals:
    // 子弹核心信号
    void bulletHit(int damage);          // 击中目标发射信号
    void bulletRecycle(BulletBase* b);   // 子弹需要回收发射信号

protected:
    double m_shootAngle; // 子弹发射角度(°)0°=向上
    int         m_imgWidth;        // 贴图宽度 有默认值
    int         m_imgHeight;       // 贴图高度 有默认值
};

#endif // BULLETBASE_H
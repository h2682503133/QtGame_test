#include "EnemyCreateTask.h"
void EnemyCreateTask::run()
{
    if (m_creator == nullptr) return;
    // 构造敌机对象
    EnemyBase* enemy = nullptr;//m_creator(m_gameWidth);
    /*
    // 敌机纯数据初始化
    enemy->setHp(0);                   // 设置血量
    enemy->setAlive(false);             // 设置存活状态
    */
    //子线程构造完成把敌机对象传回主线程
    QMetaObject::invokeMethod(m_widget, "onEnemyCreated", Qt::QueuedConnection, Q_ARG(EnemyBase*, enemy));
}
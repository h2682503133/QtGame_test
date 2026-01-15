#include "GameWidget.h"
#include "EnemyBase.h"
#include "Player.h"
#include <QThreadPool>
#include <QRunnable>
#include <QApplication>

// 子线程任务类：专门执行工厂创建敌机+构造
class EnemyCreateRunnable : public QRunnable
{
public:
    EnemyCreateRunnable(GameWidget *gameWidget) : m_gameWidget(gameWidget) {}
    void run() override {
        m_gameWidget->createEnemyByFactoryInThread();
        //构造完成发送线程安全的信号给主线程
        emit m_gameWidget->enemyCreateFinished();
    }
private:
    GameWidget *m_gameWidget;
};
#include "GameWidget.h"
#include "EnemyBase.h"

#include <QThreadPool>
#include <QRunnable>
#include <QMetaObject>

// 子线程任务类专门用于调用工厂函数+构造敌机+纯数据初始化
class EnemyCreateTask : public QRunnable
{
public:
    // 构造传参：工厂函数 + 窗口宽度 + 主线程指针 + 要创建的敌机类型
    EnemyCreateTask(EnemyCreatorFunc creator, int gameWidth, GameWidget *widget) 
        : m_creator(creator), m_gameWidth(gameWidth), m_widget(widget) {}

    // 子线程执行入口在独立子线程运行
    void run() override;
private:
    EnemyCreatorFunc m_creator;
    int m_gameWidth;
    GameWidget *m_widget;
};
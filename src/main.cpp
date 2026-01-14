#include <QApplication>
#include "GameWidget.h"

// Qt程序唯一入口主函数
int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // Qt应用程序核心对象，必须写
    
    GameWidget w; // 创建你的游戏窗口对象
    w.show();     // 显示游戏窗口
    
    return a.exec(); // 启动Qt的事件循环，程序常驻运行
}
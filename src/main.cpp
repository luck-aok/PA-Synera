#include <QApplication>
#include "gui/gamewindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); // 创建 Qt 应用

    app.setApplicationName("Synera Starter");
    app.setApplicationVersion("1.0");

    GameWindow window;// 创建主窗口
    window.setWindowTitle("Synera - Starter");
    window.resize(900, 700);
    window.show();// 显示

    return app.exec(); // 进入事件循环（一直跑，直到关闭窗口）
}

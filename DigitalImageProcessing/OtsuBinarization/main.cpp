#include "widget.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow window;
    Widget *widget = new Widget();
    window.setCentralWidget(widget);
    window.resize(900, 600);
    //window.showFullScreen();
    window.show();
    return a.exec();
}

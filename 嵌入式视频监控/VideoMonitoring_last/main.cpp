#include "mainwindow.h"
#include <QApplication>
MainWindow *w;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w=new MainWindow();
    w->show();
    w->move((QApplication::desktop()->width() - w->width())/2,
           (QApplication::desktop()->height() - w->height())/2);
    return a.exec();
}

#include "mainwindow.h"
#include "groupchatwindow/groupchatwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GroupChatWindow w;
    w.show();
    return a.exec();
}

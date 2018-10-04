#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QTabWidget>
#include "include.h"
#include "client.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QObject>
#include <iostream>
using namespace std;
QMutex myMutex;
QMutex friendMutex;



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    //Foo *f = new Foo();

    Client *c = new Client();
    //QString result = "<$FRIENDLIST$>Goldenfiber,Hoopla";
    //cout <<
//    int index = result.indexOf("$>");

//    QString from = result.mid(2, index - 2);
//    QString message = result.mid(14);
//    cout << from.toStdString() << endl;
//    cout << message.toStdString() << endl;

    return a.exec();
}

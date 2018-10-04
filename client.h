#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <iostream>
#include <fstream>
#include <QTcpSocket>
#include <QThread>
#include "mainwindow.h"
#include <QAbstractSocket>
#include <QHostAddress>
#include <QString>
#include "include.h"
#include <iostream>
#include <queue>
#include "friendmanager.h"
#include <QWebSocket>
#include <QTimer>
using namespace std;



class Client : public QObject
{
    Q_OBJECT
public:
    Client();
    ~Client();
signals:
    void friendRequest(QString);
    void friendList(QString);
    void newMessage(QString);
public slots:
    void login(QString user, QString pass);
    void connectedToHost();
    void messageReceived(QString);
    void sendMessage(QString s);
    void disconnected();
    void signUp(QString user, QString pass);
    void setSendTo(QString name);
    void prepareAndSend(QString message);
    void friendWorkerDone();
    void connectionTimedOut();
    void refresh();

private:
    QWebSocket *mainChat;
    MainWindow *w;
    bool hostConnected;
    bool loggedIn;
    vector<QString> incomingMessages;
    //vector<QString> outMessages;
    queue<QString> outgoingMessages;
    QString message; //temporary
    QString sendTo;
    QString clientName;
    bool block;
    QThread *friendWorker;
    QTimer *timer;
    QString currentStatus;
    FriendManager *fm;
};

#endif // CLIENT_H

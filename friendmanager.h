#ifndef FRIENDMANAGER_H
#define FRIENDMANAGER_H
#include <QObject>
#include "friend.h"
#include <iostream>
#include <QListWidgetItem>
#include <QString>
#include "friend.h"
#include "mainwindow.h"
#include <vector>
#include <QMutex>
using namespace std;

class FriendManager : public QObject
{
    Q_OBJECT
public:
    FriendManager();
    FriendManager(MainWindow *window);
    ~FriendManager();
public slots:
    void addFriend(Friend*);
    void recievedFriend(QString);
    void handleList(QString);
    void recievedMessage(QString);
    void changeFriend(QString, QString);
    bool friendExists(QString, Friend*&);
    void resetAll();
    void setLoggedIn(bool, QString);
    void sendMessage(QString);
private:
    MainWindow *w;
    vector<Friend*> myFriends;
    Friend  *tempFriend;
    Friend *current;
    QMutex friendLock;
    bool userLoggedIn;
    QString userName;
signals:
    void messageFromFriendManager(QString);
    void notifyClient(QString, bool);
    void updateCurrentPerson(QString);
    void enableAddFriend(bool);
    void addtoFriendList(QListWidgetItem*);
    void enableFriendList(bool);
    void changeConvo(QString);
    void clearFriendList();
    void updateChatText(QString);
    void enableSend(bool);
    void enableRefreshFL(bool);
};

#endif // FRIENDMANAGER_H

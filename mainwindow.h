#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "include.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QSize>
#include "friend.h"
class Client;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    explicit MainWindow(QWidget *parent, Client *c);
    ~MainWindow();
    Ui::MainWindow *ui;
public slots:
    void on_login_clicked();
    void on_disconnect_clicked();
    void updateServerText(QString, bool);
    bool userpassChecker(QString user, QString pass);
    void updateChatText(QString);
    void startMode();
    void userMode();
    void updateServerStatus(QString);
    void enableLoginSignup(bool);
    void enableRefresh(bool);
    void enableDisconnect(bool);
    void enableAddFriend(bool);
    void addToFriendList(QListWidgetItem*);
    void addMultipleToFriendList(QStringList);
    void clearFriendList();
    void clearChat();
    void enableFriendList(bool);
    void updateCurrentPerson(QString);
    QString getChatText();
    void changeConvo(QString);
    void enableSend(bool);
    void enableRefreshFL(bool);
private slots:
    void on_signUp_clicked();

    void on_send_clicked();

    void on_addFriendButton_clicked();

    void on_refresh_clicked();

    void on_friendListWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_refreshFriend_clicked();

private:

    Client *client;
    bool connectedToHost;
    bool connectToHostStranger;
signals:
    void changeFriend(QString, QString);
    void newMessage(QString);
    void addFriend(Friend*);
};

#endif // MAINWINDOW_H

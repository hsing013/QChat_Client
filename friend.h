#ifndef FRIEND_H
#define FRIEND_H
#include <QObject>
#include <iostream>
#include <QTextDocument>
#include <QString>
#include <QListWidgetItem>
using namespace std;

class Friend : public QObject
{
    Q_OBJECT
public:
    Friend();
    Friend(QString name, QString convo);
    Friend(QString name, QString convo, QListWidgetItem*);
    ~Friend() {}
public slots:
    QString getName();
    QString getConversation();
    void setName(QString name);
    void setConversation(QString convo);
    void setWidgetItem(QListWidgetItem *item);
    QListWidgetItem* getWidgetItem();
    void addToConvo(QString);
    void setNotification(bool);
signals:

private:
    QString conversation;
    QString name;
    QListWidgetItem *item;
};

#endif // FRIEND_H

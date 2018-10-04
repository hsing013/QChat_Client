#include "friend.h"

Friend::Friend()
{
    name = "-1";
    conversation = "-1";
    item = NULL;
}

Friend::Friend(QString name, QString convo){
    this->name = name;
    conversation = convo;
    item = NULL;
}

Friend::Friend(QString name, QString convo, QListWidgetItem *item){
    this->name = name;
    conversation = convo;
    this->item = item;
}

QString Friend::getName(){
    return name;
}

QString Friend::getConversation(){
    return conversation;
}

void Friend::setName(QString name){
    this->name = name;
}

void Friend::setConversation(QString convo){
    conversation = convo;
}

QListWidgetItem* Friend::getWidgetItem(){
    return item;
}

void Friend::setWidgetItem(QListWidgetItem *item){
    this->item = item;
}

void Friend::addToConvo(QString s){
    conversation = conversation + s;
}

void Friend::setNotification(bool flag){
    cout << "I ENTER THE WORLD NAKED\n";
    QColor red(255, 100, 100);
    QColor white(255, 255, 255);
    if (flag){
        item->setBackgroundColor(red);
    }
    else{
        item->setBackgroundColor(white);
    }

    cout << "I AM A FAILURE\n";
}

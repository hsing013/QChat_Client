#include "friendmanager.h"

FriendManager::FriendManager()
{
    w = NULL;
    tempFriend = NULL;
    current = NULL;
    userLoggedIn = false;
    userName = "";
}

FriendManager::FriendManager(MainWindow *window){
    w = window;
    tempFriend = NULL;
    current = NULL;
    connect(this, SIGNAL(updateCurrentPerson(QString)), w, SLOT(updateCurrentPerson(QString)));
    connect(this, SIGNAL(enableAddFriend(bool)), w, SLOT(enableAddFriend(bool)));
    connect(this, SIGNAL(addtoFriendList(QListWidgetItem*)), w, SLOT(addToFriendList(QListWidgetItem*)));
    connect(this, SIGNAL(enableFriendList(bool)), w, SLOT(enableFriendList(bool)));
    connect(this, SIGNAL(changeConvo(QString)), w, SLOT(changeConvo(QString)));
    connect(this, SIGNAL(clearFriendList()), w, SLOT(clearFriendList()));
    connect(this, SIGNAL(updateChatText(QString)), w, SLOT(updateChatText(QString)));
    connect(this, SIGNAL(enableSend(bool)), w, SLOT(enableSend(bool)));
    connect(this, SIGNAL(enableRefreshFL(bool)), w, SLOT(enableRefreshFL(bool)));
    emit updateCurrentPerson(QString(""));
    userLoggedIn = false;
    userName = "";
}

FriendManager::~FriendManager(){
    if (tempFriend != NULL){
        delete tempFriend;
    }

    for (unsigned i = 0; i < myFriends.size(); ++i){
        Friend *f = myFriends.at(i);
        delete f;
    }
}

void FriendManager::addFriend(Friend *f){
    friendLock.lock();
    if (w == NULL || !userLoggedIn){
        friendLock.unlock();
        return;
    }
    QString name = f->getName();
    Friend *temp = NULL;
    bool status = friendExists(name, temp);
    if (status){
        delete f;
        emit enableAddFriend(true);
        friendLock.unlock();
        tempFriend = NULL;
        return;
    }
    tempFriend = f;
    friendLock.unlock();
    emit messageFromFriendManager(QString("<$ADD$>") + name);
}


void FriendManager::recievedFriend(QString s){
    friendLock.lock();
    if (w == NULL || !userLoggedIn){
        friendLock.unlock();
        return;
    }
    QString mid = s.mid(0, 13);
    QString restOfMessage = s.mid(13);
    Friend *temp = NULL;
    if (mid == "<$ADDRESULT$>"){
        if (restOfMessage == "0"){
            QString notify = tempFriend->getName() + " was not added successfully.";
            emit notifyClient(notify, true);
            delete tempFriend;
            tempFriend = NULL;
             emit enableAddFriend(true);
        }
        else if (restOfMessage == "1"){
            bool status = friendExists(tempFriend->getName(), temp);
            if (status){
                delete tempFriend;
                tempFriend = NULL;
            }
            else{
                QString notify = tempFriend->getName() + " was added successfully.";
                emit notifyClient(notify, true);
                myFriends.push_back(tempFriend);
                emit addtoFriendList(tempFriend->getWidgetItem());
                tempFriend = NULL;
            }
             emit enableAddFriend(true);
        }
    }
    else if (mid == "<$NEWFRIEND$>"){
        bool status = friendExists(restOfMessage, temp);
        if (status){
            friendLock.unlock();
            return;
        }
        Friend *f = new Friend(restOfMessage, "", new QListWidgetItem(restOfMessage));
        QString notify = f->getName() + " just added you as a friend.";
        emit notifyClient(notify, true);
        myFriends.push_back(f);
        emit addtoFriendList(f->getWidgetItem());
    }
    else{
        cout << "ERROR in recieved friend, friend manager!!!" << endl;
    }

    friendLock.unlock();

}

bool FriendManager::friendExists(QString name, Friend *&f){
    for (unsigned i = 0; i < myFriends.size(); ++i){
        Friend *c = myFriends.at(i);
        if (c->getName() == name){
            cout << "FOUND HIM" << endl;
            f = c;
            return true;
        }
    }
    return false;
}

void FriendManager::handleList(QString s){
    friendLock.lock();
    if (w == NULL || !userLoggedIn){
        friendLock.unlock();
        return;
    }
    emit updateCurrentPerson(QString(""));
    QStringList list = s.split(",");
    emit enableFriendList(false);
    changeConvo(QString(""));
    for (unsigned i = 0; i < myFriends.size(); ++i){
        delete myFriends.at(i);
    }
    current = NULL;
    myFriends.resize(0);
    emit clearFriendList();

    for (unsigned i = 0; i < list.size(); ++i){
        QString s = list.at(i);
        if (s == "" || s == " "){
            continue;
        }
        Friend *f = new Friend(s, "", new QListWidgetItem(s));
        myFriends.push_back(f);
        emit addtoFriendList(f->getWidgetItem());
    }

    emit enableFriendList(true);
    emit enableRefreshFL(true);
    friendLock.unlock();
}

void FriendManager::recievedMessage(QString s){
    friendLock.lock();
    if (w == NULL || !userLoggedIn){
        friendLock.unlock();
        return;
    }
    int index = s.indexOf(">");

    QString from = s.mid(1, index - 1);
    QString message = s.mid(index + 1);
    Friend *f = NULL;
    bool status = friendExists(from, f);

    if (!status){
        friendLock.unlock();
        return;
    }

    if (f == current){
        emit updateChatText(s);
    }
    else{
        f->addToConvo(s);
        f->setNotification(true);
        QString notify = f->getName() + " sent you a message.";
        emit notifyClient(notify, true);
    }


    friendLock.unlock();

}

void FriendManager::changeFriend(QString s, QString convo){
    friendLock.lock();
    if (w == NULL || !userLoggedIn){
        friendLock.unlock();
        return;
    }
    Friend *f = NULL;
    bool status = friendExists(s, f);

    if (!status){
        friendLock.unlock();
        return;
    }

    if (current == NULL){
        emit enableSend(true);
        current = f;
        emit changeConvo(current->getConversation());
        f->setNotification(false);
        emit enableFriendList(true);
    }
    else{
        current->setConversation(convo);

        f->setNotification(false);
        emit changeConvo(f->getConversation());

        current = f;

        emit enableFriendList(true);
    }
    emit updateCurrentPerson(current->getName());
    friendLock.unlock();

    cout << "I make it out alive" << endl;
}

void FriendManager::resetAll(){
    for (unsigned i = 0; i < myFriends.size(); ++i){
        delete myFriends.at(i);
    }
    if (tempFriend != NULL){
        delete tempFriend;
    }
    myFriends.resize(0);
    current = NULL;
    tempFriend = NULL;
    emit updateCurrentPerson(QString(""));
    emit clearFriendList();
    emit enableSend(false);
    userName = "";
}

void FriendManager::setLoggedIn(bool flag, QString name){
    cout << "THIS HAPPENED" << endl;
    cout << flag << " " << name.toStdString() << endl;
    friendLock.lock();
    userLoggedIn = flag;
    userName = name;
    friendLock.unlock();
}

void FriendManager::sendMessage(QString m){
    cout << "FM SENDING A MESSAGE" << endl;
    friendLock.lock();
    cout << userLoggedIn << " " << userName.toStdString() << endl;
    if (w == NULL || !userLoggedIn || userName == ""){
        cout << "NOPE" << endl;
        friendLock.unlock();
        return;
    }
    QString final = "<$" + userName + ":" + current->getName() + "$>" + m;
    friendLock.unlock();
    emit messageFromFriendManager(final);

}


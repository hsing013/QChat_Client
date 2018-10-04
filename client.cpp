#include "client.h"

Client::Client()
{
    timer = new QTimer();
    currentStatus = "WAITING";
    mainChat = new QWebSocket();
    connect(mainChat, SIGNAL(connected()), this, SLOT(connectedToHost()));
    connect(mainChat, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(timer, SIGNAL(timeout()), this, SLOT(connectionTimedOut()));
    connect(mainChat, SIGNAL(textMessageReceived(QString)), this, SLOT(messageReceived(QString)));
    QUrl url(QString("ws://192.168.1.19:12345"));
    mainChat->open(url);
    w = new MainWindow(NULL, this);
    fm = new FriendManager(w);
    connect(w,  SIGNAL(changeFriend(QString, QString)), fm, SLOT(changeFriend(QString, QString)));
    connect(fm, SIGNAL(messageFromFriendManager(QString)), this, SLOT(sendMessage(QString)));
    connect(fm, SIGNAL(notifyClient(QString,bool)), w, SLOT(updateServerText(QString,bool)));
    connect(this, SIGNAL(friendRequest(QString)), fm, SLOT(recievedFriend(QString)));
    connect(this, SIGNAL(friendList(QString)), fm, SLOT(handleList(QString)));
    connect(this, SIGNAL(newMessage(QString)), fm, SLOT(recievedMessage(QString)));
    connect(w, SIGNAL(newMessage(QString)), fm, SLOT(sendMessage(QString)));
    connect(w, SIGNAL(addFriend(Friend*)), fm, SLOT(addFriend(Friend*)));
    friendWorker = new QThread();
    fm->moveToThread(friendWorker);
    hostConnected = false;
    loggedIn = false;
    sendTo = "";
    clientName = "";
    block = false;
    w->updateServerStatus(QString("Connecting..."));
    w->startMode();
    w->show();
    friendWorker->start();
    timer->start(300000);
}

Client::~Client(){
    mainChat->close();
    delete mainChat;
    friendWorker->quit();
    friendWorker->wait();
    delete fm;
}

void Client::login(QString user, QString pass){
    if (!hostConnected || loggedIn){
        return;
    }
    currentStatus = "LOGIN_WAITING";
    cout << "Made it this far2" << endl;


    QString s = "661" + user + " " + pass;
    cout << "Made it this far" << endl;
    sendMessage(s);
    clientName = user;

}

void Client::connectedToHost(){
    timer->stop();
    hostConnected = true;
    w->updateServerStatus(QString("Connected"));
    w->enableLoginSignup(true);
    currentStatus = "WAITING";
}

void Client::messageReceived(QString s){
    cout << "CLIENT " << s.toStdString() << endl;
    if (currentStatus == "LOGIN_WAITING"){
        if (s == "0"){
            w->updateServerText(QString("Log in failed. Check your username/password."), false);
            currentStatus = "WAITING";
            clientName = "";
            w->startMode();
            w->enableLoginSignup(true);
            loggedIn = false;
        }
        else if (s == "1"){
            w->updateServerText(QString("Success! Logged in as ") + clientName, false);
            currentStatus = "LOGGED_IN";
            loggedIn = true;
            w->userMode();
            fm->resetAll();
            fm->setLoggedIn(true, clientName);
            sendMessage(QString("<$GETFL$>"));
        }
        else{
            cout << "ERROR_MR()_1" << endl;
        }
    }
    else if (currentStatus == "SIGNUP_WAITING"){
        w->enableLoginSignup(true);
        if (s == "0"){
            w->updateServerText(QString("Sign up failed. Username is taken."),false);
            currentStatus = "WAITING";

        }
        else if (s == "1"){
            currentStatus = "WAITING";
            w->updateServerText(QString("Sign up successful."), false);
        }
        else{
            cout << "ERROR_MR()_2" << endl;
        }
    }
    else if (currentStatus == "LOGGED_IN"){
        QString mid = s.mid(0, 13);
        if (mid == "<$ADDRESULT$>" || mid == "<$NEWFRIEND$>"){
            emit friendRequest(s);
            return;
        }
        mid = s.mid(0, 14);
        if (mid == "<$FRIENDLIST$>"){
            s = s.mid(14);
            emit friendList(s);
            return;
        }
        emit newMessage(s);
    }
}

void Client::sendMessage(QString s){
    cout << "Sending: " << s.toStdString() << endl;
    if (!hostConnected || !mainChat->isValid()){
        cout << "NOT VALID" << endl;
        return;
    }
    mainChat->sendTextMessage(s);
}


void Client::signUp(QString user, QString pass){
    if (!hostConnected || loggedIn){
        return;
    }
    currentStatus = "SIGNUP_WAITING";
    QString s = "662" + user + " " + pass;
    cout << "Made it this far_signup" << endl;
    sendMessage(s);

}

void Client::setSendTo(QString name){
    sendTo = "<$" + clientName + ":" + name + "$>";
}

void Client::prepareAndSend(QString message){

}

void Client::friendWorkerDone(){

}

void Client::disconnected(){
    cout << "Hoopla2" << endl;
    if (timer->isActive()){
        timer->stop();
    }
    hostConnected = false;
    loggedIn = false;
    w->updateServerStatus(QString("Disconnected"));
    w->startMode();
    w->enableRefresh(true);
    clientName = "";
    fm->setLoggedIn(false, "");
    fm->resetAll();
    currentStatus = "WAITING";
}

void Client::connectionTimedOut(){
    cout << "Hoopla" << endl;
    disconnect(mainChat, 0, 0, 0);
    mainChat->abort();
    clientName = "";
    hostConnected = false;
    loggedIn = false;
    timer->stop();
    cout << "Connection timed out." << endl;
    w->updateServerStatus(QString("Disconnected"));
    w->startMode();
    w->enableRefresh(true);
    fm->setLoggedIn(false, "");
    fm->resetAll();
    currentStatus = "WAITING";
}

void Client::refresh(){
    currentStatus = "WAITING";
    fm->setLoggedIn(false, "");
    fm->resetAll();
    clientName = "";
    hostConnected = false;
    loggedIn = false;
    mainChat->abort();
    disconnect(mainChat, 0, 0, 0);
    delete mainChat;
    w->startMode();
    mainChat = new QWebSocket();
    connect(mainChat, SIGNAL(connected()), this, SLOT(connectedToHost()));
    connect(mainChat, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(mainChat, SIGNAL(textMessageReceived(QString)), this, SLOT(messageReceived(QString)));
    QUrl url(QString("ws://192.168.1.19:12345"));
    mainChat->open(url);
    if (timer->isActive()){
        timer->stop();
    }
    timer->start(15000);

}








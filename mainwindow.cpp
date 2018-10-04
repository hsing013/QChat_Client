#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"

QString convo = "Life is a hoopla\n";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectedToHost = false;
    connectToHostStranger = false;
}

MainWindow::MainWindow(QWidget *parent, Client *c) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = c;
    connectedToHost = false;
    connectToHostStranger = false;
    ui->send->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;

}



void MainWindow::on_login_clicked()
{
    QString user = ui->loginUser->text();
    QString pass = ui->loginPass->text();
    ui->loginUser->setText("");
    ui->loginPass->setText("");
    ui->login->setEnabled(false);
    ui->signUp->setEnabled(false);
    bool status = userpassChecker(user, pass);
    if (!status){
        ui->login->setEnabled(true);
        ui->login->setEnabled(true);
        return;
    }
    client->login(user, pass);

}

void MainWindow::on_disconnect_clicked()
{
    client->refresh();
}

void MainWindow::updateServerText(QString qs, bool sound){
    if (qs.at(qs.size() - 1) != '\n'){
        qs += '\n';
    }
    if (sound){
        //play sound
    }
    ui->serverText->insertPlainText(qs);
    ui->serverText->ensureCursorVisible();
}

bool MainWindow::userpassChecker(QString user, QString pass){
    if (user == "" || pass == ""){
        updateServerText(QString("Error: The username or password are empty"), false);
        return false;
    }
    else if (user.contains(" ") || pass.contains(" ")){
        updateServerText(QString("Error: The user or password has a whitespace"), false);
        return false;
    }
    else if (user.at(0).isDigit()){
        updateServerText(QString("Error: The username cannot start with a digit"), false);
        return false;
    }
    else if (!user.at(0).isLetter()){
        updateServerText((QString("Error: The username must start with an letter.")), false);
    }
    else if (user.contains("$")){
        updateServerText(QString("Error: The username cannot contain the special character $."), false);
    }
    return true;
}



void MainWindow::on_signUp_clicked()
{
    QString user = ui->signUser->text();
    QString pass = ui->signPass->text();
    ui->signUser->setText("");
    ui->signPass->setText("");
    ui->signUp->setEnabled(false);
    ui->login->setEnabled(false);
    ui->disconnect->setEnabled(false);
    bool status = userpassChecker(user, pass);
    if (!status){
        ui->signUp->setEnabled(true);
        ui->login->setEnabled(true);
        return;
    }
    client->signUp(user, pass);

}


void MainWindow::on_send_clicked()
{
    QString message = ui->chatInput->text();
//    if (message.at(message.size() - 1) != '\n'){
//        message += '\n';
//    }
    ui->chatInput->setText(QString(""));
    updateChatText(QString("<You>") + message);
    emit newMessage(message);
}

void MainWindow::updateChatText(QString qs){
    if (qs.at(qs.size() - 1) == '\n'){
        qs = qs.mid(0, qs.size() - 1);
    }
    ui->chatText->appendPlainText(qs);
    ui->chatText->ensureCursorVisible();
}



void MainWindow::on_addFriendButton_clicked()
{
    ui->addFriendButton->setEnabled(false);
    QString name = ui->addFriendInput->text();
    ui->addFriendInput->setText("");
    Friend *f = new Friend(name, "", new QListWidgetItem(name));
    emit addFriend(f);
}

void MainWindow::startMode(){
    ui->chat->setEnabled(false);
    ui->addFriend->setEnabled(false);
    ui->stranger->setEnabled(false);
    ui->login->setEnabled(false);
    ui->disconnect->setEnabled(false);
    ui->signUp->setEnabled(false);
    ui->chatText->clear();
    ui->friendListWidget->clear();
    ui->refresh->setEnabled(false);
    ui->send->setEnabled(false);
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::updateServerStatus(QString s){
    ui->serverStatus->setText(s);
}

void MainWindow::enableLoginSignup(bool flag){
    ui->login->setEnabled(flag);
    ui->signUp->setEnabled(flag);
}

void MainWindow::userMode(){
    ui->chat->setEnabled(true);
    ui->addFriend->setEnabled(true);
    ui->stranger->setEnabled(false);
    ui->login->setEnabled(false);
    ui->disconnect->setEnabled(true);
    ui->signUp->setEnabled(false);
    ui->chatText->clear();
    ui->friendListWidget->clear();
    ui->refresh->setEnabled(false);
    ui->serverText->clear();
    ui->send->setEnabled(false);
    ui->refreshFriend->setEnabled(false);
    enableAddFriend(true);
    enableFriendList(true);
}

void MainWindow::on_refresh_clicked()
{
    client->refresh();
}

void MainWindow::enableRefresh(bool flag){
    ui->refresh->setEnabled(flag);
}

void MainWindow::enableDisconnect(bool flag){
    ui->disconnect->setEnabled(flag);
}

void MainWindow::enableAddFriend(bool flag){
    ui->addFriendButton->setEnabled(flag);
}

void MainWindow::clearFriendList(){
    ui->friendListWidget->clear();
}

void MainWindow::addToFriendList(QListWidgetItem *item){
    ui->friendListWidget->addItem(item);
}

void MainWindow::addMultipleToFriendList(QStringList list){
    ui->friendListWidget->addItems(list);
}

void MainWindow::enableFriendList(bool flag){
    cout << "ENABLE FL" << endl;
    ui->friendListWidget->setEnabled(flag);
    //ui->send->setEnabled(flag);
}

void MainWindow::clearChat(){
    ui->chatText->clear();
}

void MainWindow::updateCurrentPerson(QString qs){
    cout << "UPDATE CP" << endl;
    QString s = "Talking to " + qs;
    ui->currentPerson->setText(s);
}

QString MainWindow::getChatText(){
    return ui->chatText->toPlainText();
}

void MainWindow::changeConvo(QString s){
    ui->chatText->setPlainText(s);
}

void MainWindow::on_friendListWidget_itemDoubleClicked(QListWidgetItem *item)
{
    cout << "I was triggered" << endl;
    enableFriendList(false);
    emit changeFriend(item->text(), ui->chatText->toPlainText());
}

void MainWindow::enableSend(bool flag){
    cout << "HELLO SEND WAS TRIGGERED" << endl;
    ui->send->setEnabled(flag);
}

void MainWindow::on_refreshFriend_clicked()
{
    ui->refreshFriend->setEnabled(false);
    client->sendMessage(QString("<$GETFL$>"));
}

void MainWindow::enableRefreshFL(bool flag)
{
    ui->refreshFriend->setEnabled(flag);

}

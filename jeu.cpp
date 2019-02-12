#include "jeu.h"
#include <iostream>
#include <stdlib.h>
#include <QDebug>
#include <QTime>
#include <wiringPi.h>
#include <QEvent>
#include <QDir>
#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCoreApplication>



int G_Fumee_Controler;
int G_Activ_Fumee;
int G_Activ_Cycle;
int G_Interuption_Cycle=0;
int G_DelayFumee;
int G_DelayCycle;
int G_DelaySetup;
QTime G_LastFumee;

Jeu::Jeu(IPluginsReport *report) : IPlugins(report)
{

}

bool Jeu::Game_Success()
{
    std::cout << "YOU WIN !!!" << std::endl;
    //_report->sendScore(_score); //call methode from another object not work ! logik !
    _report->gameFinished();
    return (true);
}
void Jeu::loadConfigGameTech(QString config_file)
{
    QJsonDocument docGameTech;
    QDir dirGameTech;
    qDebug() << dirGameTech.path();
    QFile fileGameTech(config_file);
    if (!fileGameTech.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          qDebug("error FFFIILLLEEEUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU");
          return;
    }
    qDebug() << "Opening config file" << config_file;
    docGameTech = QJsonDocument::fromJson(fileGameTech.readAll());
    QJsonObject object = docGameTech.object();
    qDebug("\n\n###\t start json file\t###");

    G_Activ_Cycle= object.value("Cycle").toString().toInt();
    G_Activ_Fumee = object.value("BoutonActive").toString().toInt();
    G_Fumee_Controler = object.value("EnvoiFum").toString().toInt();
    G_Interuption_Cycle = object.value("Interupt").toString().toInt();

    qDebug("###\t end json file###\n\n");

}
void Jeu::loadConfigGameFonct(QString config_file)
{
    QJsonDocument docGame;
    QDir dirGame;
    qDebug() << dirGame.path();
    QFile fileGame(config_file);
    if (!fileGame.open(QIODevice::ReadOnly | QIODevice::Text))
    {
          qDebug("error FFFIILLLEEEUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU");
          return;
    }
    qDebug() << "Opening config file" << config_file;
    docGame = QJsonDocument::fromJson(fileGame.readAll());
    QJsonObject object = docGame.object();
    qDebug("\n\n###\t start json file\t###");

    G_DelayFumee= object.value("DelayEnvoieFumee").toString().toInt();
    G_DelayCycle= object.value("DelayCycle").toString().toInt();
    G_DelaySetup= object.value("DelaySetup").toString().toInt();
    qDebug("###\t end json file###\n\n");

}
void Jeu::Pushfumee()
{
    QTime FumeeTime;
    digitalWrite(G_Fumee_Controler,HIGH);
    qDebug()<<"PushFum...";
    FumeeTime= QTime::currentTime().addSecs(G_DelayFumee);
    while (QTime::currentTime() < FumeeTime){
    //qDebug()<<"PushFum..."<<QTime::currentTime().second();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    }
    digitalWrite(G_Fumee_Controler,LOW);
    G_LastFumee =QTime::currentTime();
}
void Jeu::Interupt()
{
    qDebug()<<"interupt push";
    Pushfumee();
}
void Jeu::Setupfumee()
{
    wiringPiSetupGpio();
    loadConfigGameFonct("/home/pi/Dev/CodeSequenceur/build-generik-Desktop-Debug/GameFonct.json");
    loadConfigGameTech("/home/pi/Dev/CodeSequenceur/build-generik-Desktop-Debug/GameTech.json");
    pinMode(G_Activ_Cycle,INPUT);
    pinMode(G_Activ_Fumee,INPUT);
    pinMode(G_Fumee_Controler,OUTPUT);
    digitalWrite(G_Fumee_Controler,LOW);
    pullUpDnControl(G_Activ_Cycle,PUD_UP);
    pullUpDnControl(G_Activ_Fumee,PUD_DOWN);
    qDebug()<<G_Interuption_Cycle;

    if(wiringPiISR(G_Interuption_Cycle,INT_EDGE_FALLING, &Jeu::Interupt)<0)
        qDebug("ERRRROOOOORRRRRR");

    QTime FumeeTime;
    digitalWrite(G_Fumee_Controler,HIGH);
    FumeeTime= QTime::currentTime().addSecs(G_DelaySetup);
    while (QTime::currentTime() < FumeeTime){
    QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
    }
    digitalWrite(G_Fumee_Controler,LOW);
}



//GameFlow
void Jeu::exec()
{

    Setupfumee();
    qDebug() << "star_game";
    while(G_Activ_Cycle)
    {
        Pushfumee();
        qDebug()<<"waiting...";
        while(QTime::currentTime()<G_LastFumee.addSecs(G_DelayCycle))
        {
        //qDebug()<<"waiting..."<<QTime::currentTime().second();
        QCoreApplication::processEvents(QEventLoop::AllEvents, 1000);
        }

    }
    qDebug() << "end_game";
}

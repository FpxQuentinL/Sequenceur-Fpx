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
#include <string>



int G_Interuption=0;
int G_CSequence1=0;
int G_DelayONSequence1;
int G_DelayOFFSequence1;
int G_CSequence2=0;
int G_DelayONSequence2;
int G_DelayOFFSequence2;
int G_CSequence3=0;
int G_DelayONSequence3;
int G_DelayOFFSequence3;
int G_CSequence4=0;
int G_DelayONSequence4;
int G_DelayOFFSequence4;
QTime G_TSequence1;
QTime G_TSequence2;
QTime G_TSequence3;
QTime G_TSequence4;

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


    G_CSequence1 = object.value("ControlerSeq1").toString().toInt();
    if (object.contains("ControlerSeq2"))
    {G_CSequence2 = object.value("ControlerSeq2").toString().toInt();}
    if (object.contains("ControlerSeq3"))
    {G_CSequence3 = object.value("ControlerSeq3").toString().toInt();}
    if (object.contains("ControlerSeq4"))
    {G_CSequence4 = object.value("ControlerSeq4").toString().toInt();}
    G_Interuption = object.value("Interupt").toString().toInt();

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

    G_DelayONSequence1= object.value("DelayONSequence1").toString().toInt();
    G_DelayOFFSequence1= object.value("DelayOFFSequence1").toString().toInt();
    G_DelayONSequence2= object.value("DelayONSequence2").toString().toInt();
    G_DelayOFFSequence2= object.value("DelayOFFSequence2").toString().toInt();
    G_DelayONSequence3= object.value("DelayONSequence3").toString().toInt();
    G_DelayOFFSequence3= object.value("DelayOFFSequence3").toString().toInt();
    G_DelayONSequence4= object.value("DelayONSequence4").toString().toInt();
    G_DelayOFFSequence4= object.value("DelayOFFSequence4").toString().toInt();
    //G_DelaySetup= object.value("DelaySetup").toString().toInt();
    qDebug("###\t end json file###\n\n");

}

void Jeu::InteruptGeneral()
{
    qDebug()<<"interupt push";
    G_TSequence1=QTime::currentTime();
    G_TSequence2=QTime::currentTime();
    G_TSequence3=QTime::currentTime();
    G_TSequence4=QTime::currentTime();
    digitalWrite(G_CSequence1,LOW);
    digitalWrite(G_CSequence2,LOW);
    digitalWrite(G_CSequence3,LOW);
    digitalWrite(G_CSequence4,LOW);
}
void Jeu::InteruptSequence(int numSeq)
{
   if (numSeq==1)
   {
       G_TSequence1=QTime::currentTime();
       digitalWrite(G_CSequence1,LOW);
   }
   else if (numSeq==2)
   {
       G_TSequence2=QTime::currentTime();
       digitalWrite(G_CSequence2,LOW);
   }
   else if (numSeq==3)
   {
       G_TSequence3=QTime::currentTime();
       digitalWrite(G_CSequence3,LOW);
   }
   else if (numSeq==4)
   {
       G_TSequence4=QTime::currentTime();
       digitalWrite(G_CSequence4,LOW);
   }

}

void Jeu::Setup()
{
    wiringPiSetupGpio();
    loadConfigGameFonct("/home/pi/Dev/CodeSequenceur/build-generik-Desktop-Debug/GameFonct.json");
    loadConfigGameTech("/home/pi/Dev/CodeSequenceur/build-generik-Desktop-Debug/GameTech.json");

    pinMode(G_CSequence1,OUTPUT);
    pinMode(G_CSequence2,OUTPUT);
    pinMode(G_CSequence3,OUTPUT);
    pinMode(G_CSequence4,OUTPUT);

    digitalWrite(G_CSequence1,LOW);
    digitalWrite(G_CSequence2,LOW);
    digitalWrite(G_CSequence3,LOW);
    digitalWrite(G_CSequence4,LOW);

    G_TSequence1=QTime::currentTime();
    G_TSequence2=QTime::currentTime();
    G_TSequence3=QTime::currentTime();
    G_TSequence4=QTime::currentTime();

    if(wiringPiISR(G_Interuption,INT_EDGE_FALLING, &Jeu::InteruptGeneral)<0)
        qDebug("ERRRROOOOORRRRRR");
}



//GameFlow
void Jeu::exec()
{

    Setup();
    qDebug() << "star_game";

    while(_running)
    {

        if ((QTime::currentTime()>G_TSequence1)&&(G_CSequence1))
        {
            if (digitalRead(G_CSequence1))
            {
                digitalWrite(G_CSequence1,LOW);
                qDebug()<<"j'eteint Sequence 1";
                G_TSequence1=QTime::currentTime().addSecs(G_DelayOFFSequence1);
            }
            else
            {
                digitalWrite(G_CSequence1,HIGH);
                qDebug()<<"j'allume Sequence 1";
                G_TSequence1=QTime::currentTime().addSecs(G_DelayONSequence1);
            }
        }
        if ((QTime::currentTime()>G_TSequence2)&&(G_CSequence2))
        {
            if (digitalRead(G_CSequence2))
            {
                digitalWrite(G_CSequence2,LOW);
                qDebug()<<"j'eteint Sequence 2";
                G_TSequence2=QTime::currentTime().addSecs(G_DelayOFFSequence2);
            }
            else
            {
                digitalWrite(G_CSequence2,HIGH);
                qDebug()<<"j'allume Sequence 2";
                G_TSequence2=QTime::currentTime().addSecs(G_DelayONSequence2);
            }
        }
        if ((QTime::currentTime()>G_TSequence3)&&(G_CSequence3))
        {
            if (digitalRead(G_CSequence3))
            {
                digitalWrite(G_CSequence3,LOW);
                qDebug()<<"j'eteint Sequence 3";
               G_TSequence3=QTime::currentTime().addSecs(G_DelayOFFSequence3);
            }
            else
            {
                digitalWrite(G_CSequence3,HIGH);
                qDebug()<<"j'allume Sequence 3";
                G_TSequence3=QTime::currentTime().addSecs(G_DelayONSequence3);
            }
        }
        if ((QTime::currentTime()>G_TSequence4)&&(G_CSequence4))
        {
            if (digitalRead(G_CSequence4))
            {
                digitalWrite(G_CSequence4,LOW);
                qDebug()<<"j'eteint Sequence 4";
                G_TSequence4=QTime::currentTime().addSecs(G_DelayOFFSequence4);
            }
            else
            {
                digitalWrite(G_CSequence4,HIGH);
                qDebug()<<"j'allume Sequence 4";
                G_TSequence4=QTime::currentTime().addSecs(G_DelayONSequence4);
            }
        }








    }
    qDebug() << "end_game";
}

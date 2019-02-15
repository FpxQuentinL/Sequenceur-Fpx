#include "application.h"

#include "audiocontroller.h"
#include "server.h"
#include "jeu.h"
#include <QDebug>
#include <QFile>
#include <QJsonObject>
#include <QDir>
#include <iostream>

Application::Application(int &argc, char **argv, int flags)
:QCoreApplication(argc,argv,flags)
{
    initialize();
}

void Application::startProcess(QString /*path*/)
{

}


void Application::sendScore(int value)
{
    _server->writeSocket(QString::number(value));
}

void Application::gameFinished()
{
    _listPlugins.first()->stop();
}

bool Application::loadConfig(QString config_file)
{
    QJsonDocument doc;
    QDir            dir;
    qDebug() << dir.path();
    QFile file(config_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
          return(false);
    qDebug() << "Opening config file" << config_file;
    doc = QJsonDocument::fromJson( file.readAll() );
    QJsonObject object = doc.object();
    qDebug("\n\n###\t start json file\t###");
    for( int i=0; i<object.keys().size(); i++ )
    {
        qDebug() << object.keys()[i] << "\t" << object.value(object.keys()[i]).toString();
    }
    qDebug("###\t end json file###\n\n");

    //search key "port"
    int port = object.value("port").toString().toInt();
    if( 1024 < port && port < 65535 )
    {
        _tcpPort = port;
        std::cout << QString("Set listening port from config file : [%1] \n").arg(port).toStdString();

    }
    else
    {
        std::cout << QString("No default port define in config.json : setting default value [%1] \n").arg(_tcpPort).toStdString();
    }
    return (true);
}

bool Application::checkConfig()
{
    qDebug() << "Checking Configureation";
    if( 1024 < _tcpPort && _tcpPort < 65535 )
        if(42)
            return (true);
    qDebug() << "Error in settings !!!";
    return (false);
}

void Application::test_slot()
{
    qDebug("YOUOUOUUOOUOUOU");
    toto_signals();
}

void Application::initialize()
{
    qDebug() << "MasterApp start";
    if (!loadConfig("./test.json"))
        return;
    if (!checkConfig())
        return;


   if( !_audioController )
   {
       _audioController = new AudioController();
   }



    if( !_server )
    {
        _server = new Server(this);
        _server->initialize( _tcpPort );
        connect(_server, SIGNAL(test_signal()), this, SLOT(test_slot()));
        connect(this, SIGNAL(toto_signals()), _server, SLOT(toto_slot()));
    }
    _listPlugins.append( new Jeu(this) );
    _listPlugins.first()->setDifficulty( IPlugins::Difficulty::easy );
    //while (42) // JE SUIS PAS SUR QUE CE SOIT PAS BLOCAN !!!!
    _listPlugins.first()->start();
    connect(_listPlugins.first(),SIGNAL(Sig_Audio(QString)),_audioController,SLOT(playSound(QString)));
}

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QJsonDocument>

#include <QCoreApplication>
#include <QProcess>
#include <QList>
#include "iplugins.h"

class AudioController;
class Server;
class Application :
        public QCoreApplication,
        public IPluginsReport
{
    Q_OBJECT    //Macro for QT_slot and meta-object

public:
    Application(int &argc, char **argv, int flags = ApplicationFlags);

    void startProcess( QString path );

    virtual void sendScore(int value) override;
    virtual void gameFinished() override;
    virtual bool loadConfig( QString config_file );
    virtual bool checkConfig();

protected:
    void initialize();
    AudioController* _audioController = nullptr;
    int _tcpPort = 33333;
    Server* _server;
    QList< QProcess* > _listProcess;
    QList< IPlugins* > _listPlugins;

public slots:
    void test_slot();

signals:
    void toto_signals();

};

#endif // APPLICATION_H

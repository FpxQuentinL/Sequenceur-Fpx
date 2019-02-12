#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QList>
#include <QTcpSocket>
#include "jeu.h"

class Server : public QTcpServer

{
    Q_OBJECT

public:
    Server( QObject* parent = nullptr );
    void initialize( int tcpPort );

public slots:
    void writeSocket(QString);

protected slots:
    void acceptNextConnection();
    void readSocket();


protected:
    QList< QTcpSocket* > _sockets;

signals:
    void    test_signal();

public slots:
    void    toto_slot();

};

#endif // SERVER_H

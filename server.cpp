#include "server.h"
#include "jeu.h"
#include <thread>
#include <QTcpSocket>


Server::Server( QObject* parent )
    :QTcpServer(parent)
{

}

void Server::acceptNextConnection()
{
    QTcpSocket* socket = nextPendingConnection();
    connect( socket, SIGNAL(readyRead()), this, SLOT(readSocket()) );
    _sockets.push_back( socket );
    qDebug() << "New Client connected : ";
}

void Server::readSocket()
{
    QTcpSocket* socket = dynamic_cast< QTcpSocket* >( sender() );
    if( !socket )
        return;
    qDebug() << "socket received : " << socket->readAll();
    test_signal();
}

void Server::writeSocket(QString data)
{
    qDebug() << QString("not coded send data with socket but arg funct is :\n\t [%1] \n").arg( data );
//    _sockets.last()->write(data.toStdString());
    _sockets.last()->write("plop");
}

void Server::toto_slot()
{
    qDebug("TU GERE :D");
}

void Server::initialize( int tcpPort )
{
    if( !listen(QHostAddress::Any, tcpPort ) )
    {
        qDebug() << "Error : Unable to start listen";
    }
    qDebug() << "Start listening on port " ;
    connect( this, SIGNAL(newConnection() ), this, SLOT( acceptNextConnection() ) );
    qDebug() << "Ready for new connection" ;
}

#include "audiocontroller.h"

#include <QDebug>

AudioController::AudioController(QObject *parent)
    : QObject(parent)
{
    _player = new QMediaPlayer();
    _player->setVolume(100);

}

void AudioController::playSound(QString path)
{
    _player->setMedia(QMediaContent(QUrl(path)));
    _player->play();

    qDebug() << QString("Playing sound %1").arg( path );
}
void AudioController::testSlot()
{
    qDebug()<<"TOTO";
}

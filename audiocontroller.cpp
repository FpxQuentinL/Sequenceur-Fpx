#include "audiocontroller.h"

#include <QDebug>

AudioController::AudioController(QObject *parent)
    : QObject(parent)
{
    _player.setVolume(100);
}

void AudioController::playSound(QString path)
{
    _player.setMedia( QUrl(path) );
    _player.play();
    qDebug() << QString("Playing sound %1").arg( path );
}

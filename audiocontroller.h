#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>

#include <QtMultimedia/QtMultimedia>

class AudioController : public QObject
{
    Q_OBJECT

public:
    AudioController(QObject *parent = 0);

signals:

public slots:
    void playSound( QString path );

protected:
    QMediaPlayer _player;
};

#endif // AUDIOCONTROLLER_H

#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>

#include <QtMultimedia/QtMultimedia>

class AudioController : public QObject
{
    Q_OBJECT

public:
    AudioController(QObject *parent = 0);
public slots:
    void playSound( QString path );

    void testSlot();
protected:
    QMediaPlayer *_player;
};

#endif // AUDIOCONTROLLER_H

#ifndef JEU_H
#define JEU_H

#include "iplugins.h"
#include <QTime>



class Jeu : public IPlugins
{
    Q_OBJECT
public:
    Jeu(IPluginsReport* report);

    // QThread interface
    //bool checkVictoryCondition();
    virtual bool Game_Success();
    QString _AudioPath;
    void loadConfigGameFonct(QString config_file);
    void loadConfigGameTech(QString config_file);
    void Setup();
protected:
    virtual void exec() override;
public slots:
    static void InteruptGeneral();
    void InteruptSequence(int numSeq);
signals:
    void Sig_Audio(QString);



};



#endif // JEU_H

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
    static void Pushfumee();
    void loadConfigGameFonct(QString config_file);
    void loadConfigGameTech(QString config_file);
    void Setupfumee();
    static void Interupt();
protected:
    virtual void exec() override;


};



#endif // JEU_H

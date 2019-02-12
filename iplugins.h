#ifndef IPLUGINS_H
#define IPLUGINS_H

#include <QThread>

#include <memory>

class IPluginsReport
{
public:
    IPluginsReport() = default;

    virtual void sendScore( int value ) = 0;
    virtual void gameFinished() = 0;
};

class IPlugins : public QThread
{
public:
    enum State
    {
        Stopped = 0,
        Running,
        Success,
        Error
    };
    enum Difficulty
    {
        easy = 0,
        medium,
        hard
    };

   IPlugins( IPluginsReport* report ) { _report = report; }

   IPlugins::State      getState() { return _state; }
   IPlugins::Difficulty getDiffilcuty() {return _difficulty; }

   void setDifficulty(IPlugins::Difficulty value) { _difficulty = value; }
   bool stop()
   {
       _running = false;
       return true;
   }

protected:
    virtual void exec() = 0;
    void run()
    {
        _running = true;
        while( _running )
        {
            exec();
        }
    }
    State _state = Stopped;
    std::atomic_bool _running;
    IPluginsReport* _report = nullptr;
    Difficulty _difficulty = easy;
};
#endif // IPLUGINS_H

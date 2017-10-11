#ifndef VIDEOPRODUCE_H
#define VIDEOPRODUCE_H
#include <QThread>
class Videoproduce: public QThread
{
private:
    bool stopThreads;
protected:
    void run();
    void Savavideo();
public:
    Videoproduce();
    void stopThread(bool isStop);
};

#endif // VIDEOPRODUCE_H

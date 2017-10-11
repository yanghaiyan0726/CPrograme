#include "videoproduce.h"
#include "mainwindow.h"
extern MainWindow *w;
Videoproduce::Videoproduce()
{
    stopThreads = false;
}

void Videoproduce::Savavideo()
{
    // M->SavaVideo();
}
void Videoproduce::stopThread(bool isStop)
{
    if(isStop)
    {

    }
}
void Videoproduce::run()
{
     w->SavaVideo();
}

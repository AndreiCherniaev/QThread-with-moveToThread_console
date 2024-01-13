#include "MainClass.h"
#include <QCoreApplication>
#include <signal.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainClass mainClass(QCoreApplication::instance());

    //Connect handler like //signal(SIGINT, &SigInt_Handler);
    //thanks
    // https://forum.qt.io/topic/5927/how-to-use-sigaction-in-qt/6
    // https://stackoverflow.com/questions/54467652/how-to-set-sa-handlerint-pointer-to-function-which-is-member-of-a-class-i
    struct sigaction hup;
    hup.sa_handler = mainClass.callSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;
    if(sigaction(SIGINT, &hup, 0))
      return 1;

    return a.exec();
}

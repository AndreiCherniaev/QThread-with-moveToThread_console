#include "MainClass.h"
#include <QCoreApplication>
#include <signal.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MainClass mainClass(QCoreApplication::instance());

    //Connect handler like //signal(SIGINT, &SigInt_Handler);
    //thanks https://github.com/AndreiCherniaev/QCoreApplication_quit_example
    struct sigaction hup;
    hup.sa_handler = mainClass.callSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;
    if(sigaction(SIGINT, &hup, 0))
      return 1;

    return a.exec();
}

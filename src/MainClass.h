#pragma once

#include <QThread>
#include "Worker.h"

class MainClass : public QObject
{
    Q_OBJECT

public:
    //Дело в том, что в обработчик сигнала от Unix можно повесить только на static метод класса,
    //но мне очень захотелось повесить обработчик на non-static метод, поэтому здесь используется трюк с указателем
    //кстати обратите внимание на Rial *Rial::rialSelf; -это тоже часть этого трюка
    //https://stackoverflow.com/questions/54467652/how-to-set-sa-handlerint-pointer-to-function-which-is-member-of-a-class-i
    static MainClass* rialSelf;
    void handleSignal(const int num);
    static void setSignalHandlerObject(MainClass* newRealSelf) {
        MainClass::rialSelf= newRealSelf;
    }
    static void callSignalHandler(int num){ //num is number of handler, in case of SIGINT (Ctrl+C) it is 2
        rialSelf->handleSignal(num);
    }
    
    explicit MainClass(QObject *parent = 0);
    ~MainClass();

private slots:
    void mycallback(QStringView message);

public slots:
    void threadIsFinished();

private:
    QScopedPointer<Worker> worker;  //наш объект, в методах которого мы можем писать пользовательские алгоритмы обращаясь по необходимости ко всему богатству Qt
    QScopedPointer<QThread> thread; //наша дополнительная нить, ради которой весь сыр-бор
};

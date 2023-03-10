#pragma once

#include <QThread>
#include "Worker.h"

class Rial : public QObject
{
    Q_OBJECT

public:
    //Дело в том, что в обработчик сигнала от Unix можно повесить только на static метод класса,
    //но мне очень захотелось повесить обработчик на non-static метод, поэтому здесь используется трюк с указателем
    //кстати обратите внимание на Rial *Rial::rialSelf; -это тоже часть этого трюка
    //https://stackoverflow.com/questions/54467652/how-to-set-sa-handlerint-pointer-to-function-which-is-member-of-a-class-i
    static Rial* rialSelf;
    void handleSignal(int num);
    static void setSignalHandlerObject(Rial* newRealSelf) {
        Rial::rialSelf= newRealSelf;
    }
    static void callSignalHandler(int num){ //num is number of handler, in case of SIGINT (Ctrl+C) it is 2
        rialSelf->handleSignal(num);
    }

    explicit Rial(QObject *parent = 0);
    ~Rial();

private slots:
    void mycallback();

public slots:
    void timeout();

private:
    Worker *worker;  //наш объект, в методах которого мы можем писать пользовательские алгоритмы обращаясь по необходимости ко всему богатству Qt
    QThread *thread; //наша дополнительная нить, ради которой весь сыр-бор
};

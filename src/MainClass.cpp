#include "MainClass.h"
#include <QCoreApplication>

MainClass *MainClass::rialSelf;

MainClass::MainClass(QObject *parent) :
    QObject(parent)
  , worker(new Worker()) //родителя нет, ибо "The object cannot be moved if it has a parent."
  , thread(new QThread(this)) //родитель есть (не обязательно)
{
    qDebug() << "MainWindow::MainWindow";
    MainClass::setSignalHandlerObject(this);

    //Запуск выполнения метода run будет осуществляться по сигналу запуска от соответствующего потока
    connect(thread.get(), &QThread::started, worker.get(), &Worker::run);
    //Остановка потока же будет выполняться по сигналу finished от соответствующего объекта в потоке (то есть от worker)
    connect(worker.get(), &Worker::finished, thread.get(), &QThread::quit); //если worker сделал emit signal "finished" тогда объект thread делает quit //if worker emit signal "finished" then thread quit
    //connect(worker, &Worker::finished, worker, &Worker::deleteLater); //no need because of QScopedPointer
    //connect(thread, &QThread::finished, thread, &QThread::deleteLater); //no need because of QScopedPointer

    connect(thread.get(), &QThread::finished, this, &MainClass::threadIsFinished); //connect(worker, &Worker::destroyed, this, &QCoreApplication::quit); //if worker will destroyed then application exit(1)

    //Коннект для передачи данных из первого объекта в первом потоке, ко второму объекту во втором потоке
    connect(worker.get(), &Worker::sendMessage, this, &MainClass::mycallback, Qt::DirectConnection);
    worker->moveToThread(thread.get()); //передаём объект worker (не имеет родителя!) в нить thread

    //Устанавливаем текст в первый объект в первой нити
    worker->setMessage("Its your second thread again");

    //Запуск нити
    thread->start();

    qDebug() << "parents" << worker->parent() << thread->parent();
}


MainClass::~MainClass()
{
    qDebug() << "~MainWindow()";
}


//Обработчик случая Ctrl+C
void MainClass::handleSignal(const int num){
    qDebug()<<"Welcome to Signal handled: " << num;
    //Остановка потоков через завершение выполнения метода run в объекте worker
    thread->requestInterruption(); //worker->setRunning(false);
    qDebug()<<"requestInterruption";
}


//Обработчик вызывается когда thread finish
void MainClass::threadIsFinished()
{
    qDebug() << "threadIsFinished";
    QCoreApplication::quit();
}


void MainClass::mycallback(QStringView message)
{
    qDebug() << "Its your MainWindow with message" << message;
}

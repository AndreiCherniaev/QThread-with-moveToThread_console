#include "Rial.h"
#include <QCoreApplication>
#include <QTimer>

Rial *Rial::rialSelf;

Rial::Rial(QObject *parent) :
    QObject(parent)
  , worker(new Worker()) //родителя нет, ибо "The object cannot be moved if it has a parent."
  , thread(new QThread(this)) //родитель есть (не обязательно)
{
    qDebug() << "MainWindow::MainWindow";
    Rial::setSignalHandlerObject(this);

    //Запуск выполнения метода run будет осуществляться по сигналу запуска от соответствующего потока
    connect(thread, &QThread::started, worker, &Worker::run);
    //Остановка потока же будет выполняться по сигналу finished от соответствующего объекта в потоке (то есть от worker)
    connect(worker, &Worker::finished, thread, &QThread::quit); //если worker сделал emit signal "finished" тогда объект thread делает quit //if worker emit signal "finished" then thread quit
    connect(worker, &Worker::finished, worker, &Worker::deleteLater); //если worker сделал emit signal "finished" тогда запланировать удалить этот (worker) объект //if worker emit signal "finished" then schedules this object (worker) for deletion
    //connect(thread, &QThread::finished, thread, &QThread::deleteLater); //need or not need?

    connect(worker, &Worker::destroyed, this, &QCoreApplication::quit); //if worker will destroyed then application exit(1)

    //Коннект для передачи данных из первого объекта в первом потоке, ко второму объекту во втором потоке
    connect(worker, &Worker::sendMessage, this, &Rial::mycallback, Qt::DirectConnection);
    worker->moveToThread(thread); //передаём объект worker (не имеет родителя!) в нить thread

    //Устанавливаем текст в первый объект в первом потоке
    worker->setMessage("Its your second thread again");

    //Запуск потоков
    worker->setRunning(true);
    thread->start();

    //Таймер завершения Приложения через 5 с
    qDebug() << "timer makes setRunning(false) after 5s";
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Rial::timeout);
    timer->start(5000);

    qDebug() << "parents" << worker->parent() << thread->parent();
}

Rial::~Rial()
{
    qDebug() << "~MainWindow()";
    //delete worker; //позаботится о том, чтобы worker, у которого нет parent, был удалён, строчка connect(worker, &Worker::finished, worker, &Worker::deleteLater);
}

//Обработчик случая Ctrl+C
void Rial::handleSignal(int num){
    qDebug()<<"Welcome to Signal handled: "<<num;
    //Остановка потоков через завершение выполнения метода run в объекте worker
    worker->setRunning(false);
    qDebug()<<"wating stop action";
}

//Обработчик случая завершения по таймауту
void Rial::timeout()
{
    //Остановка потоков через завершение выполнения метода run в объекте worker
    worker->setRunning(false);
}

void Rial::mycallback()
{
    qDebug() << "Its your MainWindow";
}

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

    //Старт метода run() будет осуществляться по сигналу запуска от соответствующей нити
    connect(thread.get(), &QThread::started, worker.get(), &Worker::run);
    //Когда worker излучает finished, тогда нить прекращается (quit) //When worker emit signal "finished" then thread quit
    connect(worker.get(), &Worker::finished, thread.get(), &QThread::quit);
    //connect(worker, &Worker::finished, worker, &Worker::deleteLater); //no need because of QScopedPointer
    //connect(thread, &QThread::finished, thread, &QThread::deleteLater); //no need because of QScopedPointer

    //Основная нить анализирует, что происходит с другими, в threadIsFinished()
    connect(thread.get(), &QThread::finished, this, &MainClass::threadIsFinished);

    //Коннект для передачи данных из первого объекта в первой нити, ко второму объекту во второй нити
    connect(worker.get(), &Worker::sendMessage, this, &MainClass::mycallback, Qt::DirectConnection);

    worker->setMessage("Its your second thread again"); //Устанавливаем текст в первый объект в первой нити
    worker->moveToThread(thread.get()); //передаём объект worker (не имеет родителя!) в нить thread
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
    //Остановка нитей через завершение выполнения метода run в объекте worker
    thread->requestInterruption(); //worker->setRunning(false);
    qDebug()<<"requestInterruption";
}


//Обработчик вызывается когда thread finish
void MainClass::threadIsFinished()
{
    //If you have multimp threads you can analizy which finish
    if(thread.get()->isFinished()){
        qDebug() << "threadIsFinished";
    }
    QCoreApplication::quit();
}


void MainClass::mycallback(QStringView message)
{
    qDebug() << "Its your MainWindow with message" << message;
}

#include "Worker.h"
#include "qthread.h"
#include <QDebug>

Worker::Worker(QObject *parent) :
    QObject(parent),
    m_message("")
{
    qDebug() << "Worker::Worker";
}

bool Worker::running() const
{
    return m_running;
}

QString Worker::message() const
{
    return m_message;
}

//Самый важный метод, в котором будет выполняться "полезная" работа объекта
void Worker::run()
{
    count = 0;
    //Переменная m_running отвечает за работу объекта в потоке.
    //При значении false работа завершается
    while (m_running)
    {


        count++;
        emit sendMessage(m_message); //высылаем данные, которые будут передаваться в другой поток
        qDebug() << m_message << count;
        QThread::sleep(1); //1 s pause
    }

    emit finished();
    qDebug() << "emit finished()";
}

void Worker::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(running);
}

void Worker::setMessage(QString message)
{
    if (m_message == message)
        return;

    m_message = message;
    emit messageChanged(message);
}

Worker::~Worker()
{
    qDebug() << "~Worker()";
}

#pragma once

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
    //Свойство, управляющее работой потока
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    //Сообщение в объекте worker
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)

    bool m_running;
    QString m_message;
    int count;  //счётчик, по которому можно сомотреть сколько итераций уже произошло в ните в её while(1)

public:
    explicit Worker(QObject *parent = 0);
    ~Worker();
    bool running() const;
    QString message() const;

signals:
    void finished(); //сигнал, по которому будем завершать поток, после завершения метода run
    void runningChanged(bool running);
    void messageChanged(QString message);
    void sendMessage(QString message);

public slots:
    void run(); //метод с пользовательскими алгоритмами, которые могут быть оформлены в этом методе внутри while(1)
    void setRunning(bool running);
    void setMessage(QString message);
};

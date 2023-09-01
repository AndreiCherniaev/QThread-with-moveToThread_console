#pragma once

#include <QObject>

class Worker : public QObject
{
    Q_OBJECT
    //Сообщение в объекте worker
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged)

    QString m_message;
    int count;  //счётчик, по которому можно сомотреть сколько итераций уже произошло в ните в её while(1)

public:
    explicit Worker(QObject *parent = 0);
    ~Worker();
    QString message() const;

signals:
    void finished(); //сигнал, по которому будем завершать нить, после завершения метода run()
    void messageChanged(QString message);
    void sendMessage(QStringView message);

public slots:
    void run(); //метод с пользовательскими алгоритмами, которые могут быть оформлены в этом методе внутри while(1)
    void setMessage(QString message);
};

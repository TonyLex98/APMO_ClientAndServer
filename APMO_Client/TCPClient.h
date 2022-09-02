#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>

class TCPClient: public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = 0);

    QTcpSocket *socket_client;

public slots:
    void slot_ReadyRead();

    void slot_SendToServer(QByteArray qba_file_data);

    void slot_ConnectToServer();


private:
    QByteArray qba_file_data;

    quint32 nextBlockSize = 0;
};

#endif // TCPCLIENT_H

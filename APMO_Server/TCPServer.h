#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class TCPServer :public QTcpServer
{
    Q_OBJECT
public:
    explicit TCPServer();

private:
    QTcpSocket *tcp_socket;

    QByteArray qba_data;

    QByteArray qba_data_out;

    quint32 nextBlockSize = 0;

    void SendToClient(QByteArray qba);

signals:
    void signal_PaintImageFile(QByteArray qba);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slot_ReadyRead();

    void slot_setNextBlockSize(quint32 nextBlockSize);

};

#endif // TCPSERVER_H

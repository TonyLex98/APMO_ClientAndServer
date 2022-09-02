#include "TCPClient.h"

TCPClient::TCPClient(QObject *parent)
{
    socket_client = new QTcpSocket(this);

    connect(socket_client, &QTcpSocket::readyRead, this, &TCPClient::slot_ReadyRead, Qt::QueuedConnection);
    connect(socket_client, &QTcpSocket::disconnected,socket_client, &QTcpSocket::deleteLater);
}

void TCPClient::slot_SendToServer(QByteArray data)
{
    nextBlockSize = 0;
    qba_file_data.clear();
    QDataStream out(&qba_file_data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<quint32(0)<<data;
    out.device()->seek(0);
    out<<quint32(qba_file_data.size()-sizeof(quint32));
    socket_client->write(qba_file_data);
}

void TCPClient::slot_ConnectToServer()
{
    socket_client->connectToHost(QHostAddress::LocalHost,1111);
}

void TCPClient::slot_ReadyRead()
{
    socket_client = (QTcpSocket*)sender();
    QDataStream in(socket_client);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
        qDebug()<<"read...";
        for(;;){
            if(nextBlockSize ==0){
                if(socket_client->bytesAvailable()<2){
                    break;
                }
                in>>nextBlockSize;
            }
            if(socket_client->bytesAvailable()<nextBlockSize){
                break;
            }

            QString str;
            in>>str;
            qDebug()<<"Что-то пришло в клиент: "<< str;
            break;
        }

    }else{
        qDebug()<<"QDataStream error...";
    }
}

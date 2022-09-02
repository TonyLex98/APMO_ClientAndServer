#include "TCPServer.h"

TCPServer::TCPServer()
{
    if(this->listen(QHostAddress::Any, 1111)){
        qDebug()<<"start";

    } else{
        qDebug()<<"error";
    }

}

void TCPServer::SendToClient(QByteArray qba)
{
    qba_data_out.clear();
    QDataStream out(&qba_data_out,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out<<quint32(0)<<qba;
    out.device()->seek(0);
    out<<quint32(qba_data_out.size()-sizeof(quint32));
    tcp_socket->write(qba_data_out);
}

void TCPServer::incomingConnection(qintptr socketDescriptor)
{
    tcp_socket = new QTcpSocket();
    tcp_socket->setSocketDescriptor(socketDescriptor);
    connect(tcp_socket, &QTcpSocket::readyRead, this, &TCPServer::slot_ReadyRead);
    connect(tcp_socket, &QTcpSocket::disconnected,tcp_socket, &QTcpSocket::deleteLater);
    qDebug()<<"Клиент подключен"<< socketDescriptor;

}

void TCPServer::slot_ReadyRead()
{
    qba_data_out.clear();
    qDebug()<<"nextBlockSize = "<<nextBlockSize;

    tcp_socket = (QTcpSocket*)sender();
    QDataStream in(tcp_socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok){
        qDebug()<<"read...";
        for(;;){
            if(nextBlockSize == 0){
                if(tcp_socket->bytesAvailable()<2){
                    break;
                }
                in>>nextBlockSize;
            }
            if(tcp_socket->bytesAvailable()<nextBlockSize){
                break;
            }

            in>>qba_data_out;
            signal_PaintImageFile(qba_data_out);
            break;
        }


    }else{
        qDebug()<<"QDataStream error...";
    }

}

void TCPServer::slot_setNextBlockSize(quint32 nbs)
{
    nextBlockSize = nbs;
}

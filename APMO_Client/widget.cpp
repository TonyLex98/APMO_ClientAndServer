#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    thread_send_file = new QThread(this);
    tcp_client = new TCPClient();

    tcp_client->moveToThread(thread_send_file);

    // Соединяю сигнал destroyed со слотом quit для закрытия потока:
    connect(this, &Widget::destroyed, thread_send_file, &QThread::quit);

    // Соединяю слот передачи файла через сокет и сигнал из виджета для передачи файла в поток сетевой части
    connect(this, &Widget::signal_WriteFile,tcp_client, &TCPClient::slot_SendToServer,Qt::QueuedConnection);

    connect(this, &Widget::signal_ConnectToServer,tcp_client, &TCPClient::slot_ConnectToServer,Qt::QueuedConnection);

    connect(tcp_client->socket_client, &QTcpSocket::connected,this, &Widget::slot_SocketConnected,Qt::QueuedConnection);


    thread_send_file->start();

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_chooseFile_clicked()
{
    qba_file.clear();
    file_path = QFileDialog::getOpenFileName(this,
                                             QString::fromUtf8("Открыть файл"),
                                             QDir::currentPath(),
                                             "Images (*.png *.xpm *.jpg);;All files (*.*)");
    ui->label_file_path->setText(file_path);

    QFile file(file_path);
    if(file.open(QIODevice::ReadOnly)){
        qba_file = file.readAll();
        file.close();

        ui->pushButton_sendFile->setEnabled(true);


        QPixmap pixmap;

        if(pixmap.loadFromData(qba_file)){

            double max_size_pixmap = qMax(pixmap.width(),pixmap.height());
            double min_size_label = qMin(ui->label_image->width(),ui->label_image->height());
            double scaleFactor = min_size_label/max_size_pixmap;

            ui->label_image->setPixmap(pixmap.scaled(pixmap.width()*scaleFactor,pixmap.height()*scaleFactor));
        }
    }
    else{
        QMessageBox::warning(this, "Внимание!", "Файл не открылся!");
    }
}


void Widget::on_pushButton_sendFile_clicked()
{
    // Отправляю файл в поток сетевой части:
    emit signal_WriteFile(qba_file);
}


void Widget::on_pushButton_connect_to_server_clicked()
{
    emit signal_ConnectToServer();
}

void Widget::slot_SocketConnected()
{
    QMessageBox::warning(this, "Успех!", "Вы подключены!");

    ui->pushButton_chooseFile->setEnabled(true);
}


#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcp_server = new TCPServer();
    thread_read_file = new QThread(this);

    //udp_server->moveToThread(thread_read_file);
    tcp_server->moveToThread(thread_read_file);

    // Соединяю сигнал destroyed со слотом quit для закрытия потока:
    connect(this, &Widget::destroyed, thread_read_file, &QThread::quit);

    connect(tcp_server, &TCPServer::signal_PaintImageFile, this, &Widget::slot_PaintImageFile, Qt::QueuedConnection);

    connect(this, &Widget::signal_setNextBlockSize,tcp_server, &TCPServer::slot_setNextBlockSize);

    thread_read_file->start();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::slot_PaintImageFile(QByteArray qba)
{
    QPixmap pixmap;
    if(pixmap.loadFromData(qba)){



        double max_size_pixmap = qMax(pixmap.width(),pixmap.height());
        double min_size_label = qMin(ui->label_image->width(),ui->label_image->height());
        double scaleFactor = min_size_label/max_size_pixmap;

        ui->label_image->setPixmap(pixmap.scaled(pixmap.width()*scaleFactor,pixmap.height()*scaleFactor));

    }

    emit signal_setNextBlockSize(0);

}


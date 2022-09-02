#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <QFileDialog>
#include <QMessageBox>
#include <TCPClient.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

signals:
    void signal_WriteFile(QByteArray qba);

    void signal_ConnectToServer();

private slots:
    void on_pushButton_chooseFile_clicked();

    void on_pushButton_sendFile_clicked();

    void on_pushButton_connect_to_server_clicked();

public slots:
    void slot_SocketConnected();


private:
    Ui::Widget *ui;
    QThread *thread_send_file;
    TCPClient *tcp_client;

    QByteArray qba_file;
    QString file_path;
};
#endif // WIDGET_H

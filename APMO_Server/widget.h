#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include <TCPServer.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void slot_PaintImageFile(QByteArray qba);

signals:
    void signal_setNextBlockSize(quint32 nextBlockSize);

private:
    Ui::Widget *ui;
    QThread *thread_read_file;
    TCPServer *tcp_server;

};
#endif // WIDGET_H

#ifndef RECEIVER_H
#define RECEIVER_H

#include <QObject>
#include <QWidget>
#include <QtNetwork>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QUdpSocket;
class QAction;
QT_END_NAMESPACE

class Receiver : public QWidget
{
    Q_OBJECT
public:
    explicit Receiver(QWidget *parent = 0);

private slots:
    void changePicture();
    void copyPicture();

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    QUdpSocket *udpSocket1;
    QUdpSocket *udpSocket2;
    QGraphicsScene *scene;
    QGraphicsView *view;
};

#endif


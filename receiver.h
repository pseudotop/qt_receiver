#ifndef RECEIVER_H
#define RECEIVER_H

#include <QApplication>
#include <QObject>
#include <QWidget>
#include <QDesktopWidget>
#include <QtNetwork>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
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

private:
    void setScaledPixmap(const QString& str);
    QImage *inputImg;
    QPixmap *buffer;
    QLabel *viewLabel;
    QPushButton *quitButton;
    QUdpSocket *udpSocket;
    QGraphicsScene *scene;
    QGraphicsView *view;
    int screenWidth;
    int screenHeight;
};

#endif


#include "receiver.h"

Receiver::Receiver(QWidget *parent) :
 QWidget(parent)
{
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(QHostAddress::Any, 54321, QUdpSocket::ShareAddress);
	udpSocket->joinMulticastGroup(QHostAddress::Any);
	
	scene = new QGraphicsScene();
	view = new QGraphicsView();
	view->setBackgroundBrush(QBrush(Qt::black));
	view->setScene(scene);
	view->show();
	view->showFullScreen();
	
	view->setStyleSheet( "QGraphicsView { border-style: none; }" );
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setCursor(Qt::BlankCursor);
	view->fitInView(scene->itemsBoundingRect(), Qt::KeepAspectRatio);

	connect(udpSocket, SIGNAL(readyRead()), this, SLOT(changePicture()));
}

void Receiver::changePicture()
{
	while(udpSocket->hasPendingDatagrams()){
		QByteArray datagram;
		datagram.resize(udpSocket->pendingDatagramSize());

		QHostAddress sender;
		quint16 senderPort;
		QStringList ipsplit;
		QString num;

		udpSocket->readDatagram(datagram.data(), datagram.size(),&sender,&senderPort);
		foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
			if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
				qDebug() << "client address: " << address.toString();
				ipsplit = address.toString().split(".");
				qDebug() << "last ip: " << ipsplit;
			}
		}
		if (ipsplit[2].toInt() == 1){
			if (ipsplit[3].toInt() == 41){
				datagram.replace(QByteArray("/mnt/pishare"),QByteArray("/home/share"));
			}
			ipsplit[3] = QString::number(254+ipsplit[3].toInt());
		}
			
		QString x;
		x = QString("%1/%2_%3.jpg").arg(datagram,QString::fromAscii(datagram).section("/",-1,-1),ipsplit[3]);
		
		QFileInfo file(x);
		if (file.isFile()){
			qDebug() << "Message from: " << sender.toString();
			qDebug() << "Message port: " << senderPort;
			qDebug() << "Message: " << datagram;
			qDebug() << "abspath: "<< x;
			scene->addPixmap(QPixmap(x));
		}
		else {
			qDebug() << "!!There is nothing to look for: " << x;
			return;
		}
	}
}

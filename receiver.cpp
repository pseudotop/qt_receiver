#include "receiver.h"

Receiver::Receiver(QWidget *parent) :
 QWidget(parent)
{
	udpSocket1 = new QUdpSocket(this);
	udpSocket2 = new QUdpSocket(this);
	udpSocket1->bind(QHostAddress::Any, 54321, QUdpSocket::ShareAddress);
	udpSocket1->joinMulticastGroup(QHostAddress::Any);
	udpSocket2->bind(QHostAddress::Any, 54322, QUdpSocket::ShareAddress);
	udpSocket2->joinMulticastGroup(QHostAddress::Any);
	
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

	connect(udpSocket1, SIGNAL(readyRead()), this, SLOT(changePicture()));
	connect(udpSocket2, SIGNAL(readyRead()), this, SLOT(copyPicture()));
}

void Receiver::changePicture()
{
	while(udpSocket1->hasPendingDatagrams()){
		QByteArray datagram;
		datagram.resize(udpSocket1->pendingDatagramSize());

		QHostAddress sender;
		quint16 senderPort;
		QStringList ipsplit;

		udpSocket1->readDatagram(datagram.data(), datagram.size(),&sender,&senderPort);
		foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
			if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
				qDebug() << "client address: " << address.toString();
				ipsplit = address.toString().split(".");
				qDebug() << "last ip: " << ipsplit;
			}
		}
		QString inputData;
		inputData=QString::fromAscii(datagram);	
		if (inputData.indexOf("\\") > 0){
			qDebug() << "passed on Windows";
			qDebug() << "inputData " << inputData;
			qDebug() << "replace" << inputData.replace(QString("\\"),QString("/")).section("/",1,-1);
			inputData=inputData.replace(QString("\\"),QString("/")).section("/",1,-1);
			inputData=QString("/media/pishare-windows/").append(inputData);
		}
		return;
		if (ipsplit[2].toInt() == 1){
			ipsplit[3] = QString::number(254+ipsplit[3].toInt());
		}
		QString tmpfileName1 = QDir(inputData).entryList(QDir::Files).at(0);
		//QString tmpfileName2 = QDir(inputData).entryList(QDir::Files).at(1);
		QStringList fileName(tmpfileName1.split("_"));
		//QStringList compareName(tmpfileName2.split("_"));
		//for(int i=0,i<fileName.count(),++i){
			//if(fileName.at(i).contains(compareName.at(i)))
		//}
		QString copyData = QString("%1/%2_%3_%4_%5_%6").arg(inputData,fileName[0],fileName[1],QString::number(ipsplit[3].toInt()-1),fileName[3],fileName[4]);
		inputData = QString("~/%1_%2_%3_%4_%5").arg(fileName[0],fileName[1],QString::number(ipsplit[3].toInt()-1),fileName[3],fileName[4]);
		
		qDebug() << "inputData" << inputData;
		qDebug() << "copyData" << copyData;

		if(!QFileInfo(inputData).isFile()){
			QFileInfo file(copyData);
			if (file.isFile()){
				QFile::copy(copyData, inputData);
			} else {
				qDebug() << "!!There is nothing to look for: " << inputData;
				return;
			}
		}
		
		qDebug() << "Message from: " << sender.toString();
		qDebug() << "Message port: " << senderPort;
		qDebug() << "Message: " << datagram;
		qDebug() << "abspath: "<< inputData;
		scene->addPixmap(QPixmap(inputData));
		
	}
}

void Receiver::copyPicture()
{
	while(udpSocket2->hasPendingDatagrams()){
		QByteArray datagram;
		datagram.resize(udpSocket2->pendingDatagramSize());

		QHostAddress sender;
		quint16 senderPort;
		QStringList ipsplit;

		udpSocket2->readDatagram(datagram.data(), datagram.size(),&sender,&senderPort);
		QProcess *wget = new QProcess;
		wget->setProgram("wget");
		QStringList arguments;
		arguments << QString::fromAscii(datagram) << "-P /home/pi/";
		wget->setArguments(arguments);
		wget->deleteLater();
		wget->start();

		qDebug() << "Message from: " << sender.toString();
		qDebug() << "Message port: " << senderPort;
		qDebug() << "Message: " << datagram;
	}	
}

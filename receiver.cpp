#include "receiver.h"

Receiver::Receiver(QWidget *parent) :
 QWidget(parent)
{
	QRect screen = QApplication::desktop()->screenGeometry();
	screenHeight = screen.height();
	screenWidth = screen.width();
	
	//this->showFullScreen();
	udpSocket = new QUdpSocket(this);
	udpSocket->bind(QHostAddress::Any, 54321, QUdpSocket::ShareAddress);
	udpSocket->joinMulticastGroup(QHostAddress::Any);

	inputImg = new QImage();	
	buffer = new QPixmap();
	viewLabel = new QLabel(this);

	QApplication::setOverrideCursor(Qt::BlankCursor);
	this->setStyleSheet("background-color:black;");

        viewLabel->show();
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

		udpSocket->readDatagram(datagram.data(), datagram.size(),&sender,&senderPort);
		foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
			if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost)) {
				if (address.toString().split(".")[2].toInt()>1){
					continue;
				}
				qDebug() << "client address: " << address.toString();
				ipsplit = address.toString().split(".");
				if (ipsplit[2].toInt() == 1){
					ipsplit[3] = QString::number(253+ipsplit[3].toInt());
				}
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
			inputData=QString("/media/pishare-server/").append(inputData);
		}
		qDebug() << "inputData: " << inputData;
		//qDebug() << "files in inputData: " << QDir(inputData).entryList(QDir::Files);
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

		/*
		if(!QFileInfo(inputData).isFile()){
			QFileInfo file(copyData);
			if (file.isFile()){
				QFile::copy(copyData, inputData);
			} else {
				qDebug() << "!!There is nothing to look for: " << inputData;
				return;
			}
		}
		*/	
		qDebug() << "Message from: " << sender.toString();
		qDebug() << "Message port: " << senderPort;
		qDebug() << "Message: " << datagram;
		qDebug() << "abspath: "<< inputData;
		
		setScaledPixmap(copyData);	
	}
}

void Receiver::setScaledPixmap(const QString& str){
	inputImg->load(str);
	*buffer = QPixmap::fromImage(*inputImg); 
	*buffer = buffer->scaled(screenWidth,screenHeight,Qt::KeepAspectRatio,Qt::FastTransformation);
	qDebug() << "screen(width,height): "<< screenWidth << screenHeight;
	qDebug() << "image(width,height): "<< inputImg->width() << inputImg->height();

        //viewLabel->setAlignment(Qt::AlignCenter);
        viewLabel->setPixmap(*buffer);
	viewLabel->adjustSize();
	viewLabel->move(0,0);
        viewLabel->setScaledContents(true);
}

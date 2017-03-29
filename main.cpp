#include "widget.h"
#include "receiver.h"
#include <iostream>

#include <QApplication>

int main(int argc, char *argv[]){
	QApplication a(argc, argv);
	Receiver receiver;
	receiver.showFullScreen();

	return a.exec();
}

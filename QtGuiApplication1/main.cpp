#include "MyWord.h"
#include <QtWidgets/QApplication>
#include "logmanager.h"
#include <iostream>
#include <QDebug>
void myMessageOutPut(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
	LogManager::instance().logMessage(type, context, msg);
	std::cout << msg.toLocal8Bit().data() << std::endl << std::flush;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//ÈÕÖ¾¿ªÆô
	qInstallMessageHandler(myMessageOutPut);
	LogManager::instance().start();
	MyWord w;
	w.show();
	return a.exec();
}
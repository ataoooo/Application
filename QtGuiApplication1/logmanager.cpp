#include "logmanager.h"
#include <QDateTime>
#include <thread>
#include <chrono>
#include <QDir>
#include <QString>
#include <QMutexLocker>
#include <iostream>

void LogManager::run()
{
	size_t index = NULL;
	while (!m_bExit.load())
	{
		m_mutex.lock();
		if (m_logMsgQueue.size() <= 0)
		{
			m_mutex.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		QByteArray msg = m_logMsgQueue.front().toUtf8();
		m_logMsgQueue.pop_front();
		m_mutex.unlock();
		m_logFile->write(msg.data(), msg.size());
		if (m_sendMessageToStdOut)
		{
			printf("%s", msg.data());
		}
		m_logFile->flush();
	}
}
LogManager::LogManager()
{
	QDir().mkdir("./MyLog");
	QString logFileName = QString("./MyLog/%1_log.txt").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd-mm-ss"));
	m_logFile = std::make_unique<std::fstream>(logFileName.toStdWString(), std::ios::out | std::ios::app);
}

LogManager::~LogManager()
{
	const char str[] = "LOGManager destroy\n";
	m_logFile->write(str, sizeof(str));
	m_logFile->close();
	stop();
}

LogManager& LogManager::instance()
{
	static LogManager logManager;
	return logManager;
}

void LogManager::logMessage(QtMsgType type, const QMessageLogContext & context, const QString & msg)
{
	if (m_bExit.load())
		return;
	QString text;
	switch (type)
	{
	case QtDebugMsg:
		text = QStringLiteral("Debug:");
		break;
	case QtWarningMsg:
		text = QStringLiteral("Warning:");
		break;
	case QtCriticalMsg:
		text = QStringLiteral("Critial:");
		break;
	case QtFatalMsg:
		text = QStringLiteral("Fatal:");
		break;
	default:
		break;
	}

	//时间
	auto timeStr = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

	//线程ID
	auto threadID = (int)currentThreadId();

	//文件
	auto index = QString(context.file).lastIndexOf("\\") + 1;
	auto index1 = QString(context.file).lastIndexOf("/") + 1;
	auto indexMax = index > index1 ? index : index1;
	auto fileStr = QString(context.file).remove(NULL, indexMax);

	//行数
	auto lineStr = context.line;

	QString message = QString("[%1 %2 %3 %4(%5)] %6\n")
		.arg(text)
		.arg(timeStr)
		.arg(threadID)
		.arg(fileStr)
		.arg(lineStr)
		.arg(msg);
	logMessage(message);
}

void LogManager::logMessage(const QString& msg)
{
	QMutexLocker locker(&m_mutex);
	if (m_logMsgQueue.size() > 5000)
	{
		m_logMsgQueue.pop_front();
	}
	m_logMsgQueue.push_back(msg);
}

void LogManager::stop()
{
	QMutexLocker locker(&m_mutex);
	m_bExit = true;
	QThread::quit();
	QThread::wait();
}
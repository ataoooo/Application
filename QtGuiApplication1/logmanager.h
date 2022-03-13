#ifndef LOGMANAGER_H
#define LOGMANAGER_H
//日志管理类
#include <QString>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <atomic>
#include <fstream>
#include <memory>
#include <QMutex>
class LogManager : public QThread
{
	Q_OBJECT
public:
	static LogManager& instance();
	void logMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg);
	void logMessage(const QString& msg);
	void stop();
	void sendMessageToDbw(const bool send) { m_sendMessageToDbw = send; }
	void sendMessageToStdOut(const bool send) { m_sendMessageToStdOut = send; }
protected:
	virtual void run();
private:
	LogManager();
	LogManager(const LogManager&);
	~LogManager();

	std::atomic<bool> m_sendMessageToDbw = false;
	std::atomic<bool> m_sendMessageToStdOut = false;
	std::atomic<bool> m_bExit = false;
	std::unique_ptr<std::fstream> m_logFile;
	QMutex m_mutex;
	QStringList m_logMsgQueue;
};

#endif // !LOGMANAGER_H

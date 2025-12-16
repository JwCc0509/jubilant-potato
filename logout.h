#ifndef _LOGOUT_H_
#define _LOGOUT_H_
#include <QApplication>
#include <QFont>
#include <QFile>
#include <QDebug>
#include <QString>
#include <QDateTime>
#include <QMutexLocker>

void LogOut(QtMsgType type,const QMessageLogContext &context,const QString &msg);
//void installLogOut(void);

#endif // _logout_h_

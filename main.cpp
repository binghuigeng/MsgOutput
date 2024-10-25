#include "mainwindow.h"

#include <QApplication>
//#include <QTextCodec>
#include <QtMessageHandler>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <iostream>

void writeLog(const QString &msg)
{
    QString dirPath = QString("./Log/%1").arg(msg.mid(1, 7));
    QDir dir;
    if(!dir.exists(dirPath))
    {
        dir.mkpath(dirPath);
    }

    QString fileName = QString("%1/%2.log").arg(dirPath, msg.mid(1, 10));
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        QTextStream text_stream(&file);
        text_stream << msg << endl;
        file.close();
        //打印到控制台
#ifdef QT_DEBUG
        std::cout << msg.toLocal8Bit().constData() << std::endl;
//        std::cout << msg.toUtf8().constData() << std::endl;
#endif
    }
    else {
        std::cout << QString("Cannot write file %1:%2").arg(fileName, file.errorString()).toStdString() << std::endl;
    }
}


void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {
     writeLog(qFormatLogMessage(type, context, msg));
 }

int main(int argc, char *argv[])
{
    // 解决中文乱码问题
//    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//    QTextCodec::setCodecForLocale(codec);

    // 更改默认消息处理程序的输出
#ifdef QT_DEBUG
    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] [%{type}] [%{threadid}] %{file}:%{line}:%{function} %{message}");
#else
    qSetMessagePattern("[%{time yyyy-MM-dd hh:mm:ss.zzz}] [%{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}] [%{threadid}] %{file}:%{line}:%{function} %{message}");
#endif
    qInstallMessageHandler(myMessageOutput);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // 打印信息
    qDebug("This is a debug message.");
    qInfo("This is a info message.");
    qWarning("This is a warning message.");
    qCritical("This is a critical message.");
//    qFatal("This is a fatal message.");
    qDebug("Hello World 耿兵辉 You are great.");

    return a.exec();
}

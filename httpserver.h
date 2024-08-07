#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "serverauth.h"

class HttpServer : public QTcpServer {
    Q_OBJECT
public:
    HttpServer(QObject *parent = nullptr);
    QString toClientMessage(QString);
    void addHttpServer(QString type,QString path,QString ContentType,std::function<QString(QString)>fun);
private slots:
    void handleNewConnection();
    QByteArray handleGet(const QString &path, const QString &GetBody);

    QByteArray handlePost(const QString &path, const QStringList &requestLines);

    QByteArray handlePut(const QString &path, const QStringList &requestLines);

    QByteArray handleDelete(const QString &path);
private:
     QVector<ServerAuth*>ServerList;
     ServerAuth*Server;
     int pathSize;
     int newSize;
     QByteArray boundary;
     bool check;
     QMap<QString,QString>FromData;
private:
     QStringList fromdata(QByteArray request,QStringList requestLines);
};

#endif // HTTPSERVER_H

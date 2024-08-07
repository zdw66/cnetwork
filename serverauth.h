#ifndef SERVERAUTH_H
#define SERVERAUTH_H

#include <QDebug>

class ServerAuth
{
public:
    ServerAuth(QString path,QString type,QString ContentType,std::function<QString(QString)>fun);
    QString Serverpath;
    QString Servertype;
    QString ContentType;
    std::function<QString(QString)>action;
};

#endif // SERVERAUTH_H

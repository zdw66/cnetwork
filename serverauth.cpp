#include "serverauth.h"

ServerAuth::ServerAuth(QString path,QString type,QString ContentType,std::function<QString(QString)>function)
{
    this->Serverpath=path;
    this->Servertype=type;
    this->ContentType=ContentType;
    this->action=function;
}

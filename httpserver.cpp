#include "httpserver.h"

HttpServer::HttpServer(QObject *parent)
    : QTcpServer(parent){
    connect(this, &QTcpServer::newConnection, this, &HttpServer::handleNewConnection);
    Server = new ServerAuth("","","",nullptr);
}

void HttpServer::handleNewConnection(){
    QTcpSocket *clientSocket = nextPendingConnection();

    connect(clientSocket, &QTcpSocket::readyRead, this, [this, clientSocket]() {
        QByteArray request = clientSocket->readAll();
        // 解析请求
        QStringList requestLines = QString::fromUtf8(request).split("\r\n");
        QString requestLine = requestLines.first();
        QStringList requestTokens = requestLine.split(" ");
        QString GetBody;
        if (requestTokens.size() >= 3) {
            QString method = requestTokens[0]; // 请求方法
            QString path = requestTokens[1];   // 请求路径
            if(method=="GET"){
                QString data = path.split("?")[1];
                QStringList bodyList;
                if(data.contains("&")) bodyList = data.split("&");
                else bodyList.append(data);
                QJsonObject jsonObject;
                foreach (auto var, bodyList) {
                   QStringList list = var.split("=");
                   jsonObject[list[0]]=list[1];
                }
                QJsonDocument jsonDoc(jsonObject);

                GetBody = jsonDoc.toJson(QJsonDocument::Compact);
                path = path.split("?")[0].chopped(1);
            }
            foreach (auto var, ServerList) {
                if(var->Serverpath==path&&var->Servertype==method){
                    this->Server=var;
                    break;
                }
            }
            requestLines = fromdata(request,requestLines);
            QByteArray response;
            if (Server->Servertype == "GET"&&method==Server->Servertype) {
                response = handleGet(path,GetBody);
            } else if (Server->Servertype == "POST"&&method==Server->Servertype) {
                response = handlePost(path, requestLines);
            } else if (Server->Servertype == "PUT"&&method==Server->Servertype) {
                response = handlePut(path, requestLines);
            } else if (Server->Servertype == "DELETE"&&method==Server->Servertype) {
                response = handleDelete(path);
            } else {
                response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n";
            }
            clientSocket->write(response);
         }
        clientSocket->disconnectFromHost();
    });
    connect(clientSocket, &QTcpSocket::disconnected, clientSocket, &QTcpSocket::deleteLater);
    }

QByteArray HttpServer::handleGet(const QString &path, const QString &GetBody) {
    // 处理GET请求
    if (path == Server->Serverpath) {
         return "HTTP/1.1 200 OK\r\nContent-Type: "+Server->ContentType.toUtf8()+"\r\n\r\n" + Server->action(GetBody).toUtf8();
     }
     return "HTTP/1.1 404 Not Found\r\n\r\n";
}

QByteArray HttpServer::handlePost(const QString &path, const QStringList &requestLines) {
    // 处理POST请求，通常请求体在请求头之后
    if (path == Server->Serverpath) {
        // 解析请求体
        QString body;
        if (requestLines.size() > 1) {
            int index=0;
            while(!requestLines.at(index).contains("{"))index++;
            for(;index<requestLines.size();index++){
                body+=requestLines.at(index);
            }
         }
         return "HTTP/1.1 201 Created\r\nContent-Type: "+Server->ContentType.toUtf8()+"\r\n\r\n" + Server->action(body).toUtf8();
     }
     return "HTTP/1.1 404 Not Found\r\n\r\n";
 }

 QByteArray HttpServer::handlePut(const QString &path, const QStringList &requestLines) {
     // 处理PUT请求
     if (path == Server->Serverpath) {
         // 解析请求体
         QString body;
         if (requestLines.size() > 1) {
             int index=0;
             while(!requestLines.at(index).contains("Content-Type"))index++;
             index++;
             for(;index<requestLines.size();index++){
                 body+=requestLines.at(index);
             }
          return "HTTP/1.1 200 OK\r\nContent-Type: "+Server->ContentType.toUtf8()+"\r\n\r\n" + Server->action(body).toUtf8();
          }
          return "HTTP/1.1 404 Not Found\r\n\r\n";
     }
 }

 QByteArray HttpServer::handleDelete(const QString &path) {
     // 处理DELETE请求
     if (path == Server->Serverpath) {
         return "HTTP/1.1 200 OK\r\nContent-Type: "+Server->ContentType.toUtf8()+"\r\n\r\n{\"message\":\"DELETE response\"}";
     }
     return "HTTP/1.1 404 Not Found\r\n\r\n";
 }

 QStringList HttpServer::fromdata(QByteArray request,QStringList requestLines){
     if(request.indexOf("Content-Type: multipart/form-data;")!=-1){
         for(int i =0;i<requestLines.size();i++){
             if(requestLines[i].contains("Content-Disposition:")){
                 QStringList DataList = requestLines[i].split("\"");
                 i+=2;
                 FromData[DataList[DataList.size()-2]]=requestLines[i];
             }
         }
         QStringList dataList;
         dataList.append("{");
         for (auto it = FromData.constBegin(); it != FromData.constEnd(); ++it) {
                 dataList.append("\""+it.key()+"\"" + ":" +"\""+it.value()+"\"");
                 dataList.append(",");
         }
         dataList.removeAt(dataList.size()-1);
         dataList.append("}");
         return dataList;
     }else return requestLines;

 }


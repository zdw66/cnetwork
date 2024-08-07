# cnetwork

#### 介绍
基于QT/C++开发的WEB框架

#### 软件架构
QT原生态，无第三方库


#### 安装教程

1.  进行构建
2.  将构建生成的.os或者.dll文件及头文件引入

#### 使用说明

   
```
HttpServer *server = new HttpServer();//服务器初始化
//（参数说明：请求形式，url，返回数据形式，对数据操作的函数）
server->addHttpServer("POST","/api/data","application/json",[&](QString data){
    return data;
});
/*请求形式：http://192.168.12.129:8080/api/data
* 请求体
* {
        "name": 1,
        "dsa": [
            "sad",
            "ad"
        ],
        "s": {
            "a": 1
        }
    }或者from-data形式（仅支持Text）
*/
server->addHttpServer("GET","/api/ss","text/html",[&](QString data){
    return data;
});
/*
*请求形式：http://192.168.12.129:8080/api/ss/?a=1&b=3
*/
//开启对某个端口的监听
if (!server->listen(QHostAddress::Any, 8080)) {
    qFatal("Unable to start the server: %s.", server->errorString().toUtf8().constData());
}qDebug() << "Server started on port 8080.";
```
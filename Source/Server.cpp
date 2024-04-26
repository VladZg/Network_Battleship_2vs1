#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QNetworkAccessManager manager;
    QNetworkRequest request(QUrl("https://www.google.com"));
    QNetworkReply *reply = manager.get(request);

    QObject::connect(reply, &QNetworkReply::finished, [&](){
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            qDebug() << "Response:" << data;
        } else {
            qDebug() << "Error:" << reply->errorString();
        }
        app.quit();
    });

    return app.exec();
}

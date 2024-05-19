#include <QTcpSocket>
#include <QDebug>

int main(int argc, char *argv[]) {
  // Создаем сокет клиента
  QTcpSocket client;

  // Подключаемся к серверу
  client.connectToHost("127.0.0.1", 12345);

  if (client.state() == QAbstractSocket::ConnectedState) {
    qDebug() << "Подключено к серверу";

    // Можно выполнять какие-либо действия с сервером
    //    ...

    // Закрываем сокет клиента
    client.close();
  } else {
    qDebug() << "Не удалось подключиться к серверу";
  }

  return 0;
}

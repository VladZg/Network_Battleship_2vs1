#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

int main(int argc, char *argv[]) {
  // Создаем сервер
  QTcpServer server;

  // Связываем сервер с портом
  if (!server.listen(QHostAddress::LocalHost, 12345)) {
    qDebug() << "Не удалось связать сервер с портом";
    return 1;
  }

  // Ждем входящих подключений
  while (true) {
    // Блокирующий вызов, ожидающий входящих подключений
    QTcpSocket *client = server.nextPendingConnection();

    if (client) {
      // Клиент подключился
      qDebug() << "Клиент подключился";

      // Закрываем сокет клиента
      client->close();
    }
  }

  return 0;
}


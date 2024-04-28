#include "client.h"

Client::Client()
{

}

Client::~Client() {}

QString Client::getLogin()
{
    return login_;
}

void Client::setLogin(const QString& login)
{
    login_ = login;
}

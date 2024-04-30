#include "client.h"

Client::Client() :
    field_(NULL)
{

}

Client::~Client()
{
    if (field_)
        delete field_;
}

QString Client::getLogin()
{
    return login_;
}

void Client::setLogin(const QString& login)
{
    login_ = login;
}

void Client::initField()
{
    field_ = new Field();
}

void Client::initField(QString field)
{
    field_ = new Field(field);
}

QString Client::getFieldStr()
{
    if (!field_)
        return NULL;

    return field_->getFieldStr();
}

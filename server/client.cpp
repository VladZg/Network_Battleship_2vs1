#include "client.hpp"

Client::Client() :
    field_()
{

}

Client::~Client()
{
    if (field_)
        delete field_;
}

Field Client::getField()
{
    return *field_;
}

void Client::setFieldDraw(QVector<Field::CellDraw> field)
{
    field_->setFieldDraw(field);
}

QString Client::getLogin()
{
    return login_;
}

bool Client::isAuthorized() const
{
    return status_ == ST_AUTHORIZED;
}

void Client::updateState(ClientStatus state)
{
    status_ = state;
}

void Client::setLogin(const QString& login)
{
    login_ = login;
}

void Client::initField()
{
    field_ = new Field();
    field_->initFieldDraw();
}

void Client::initField(QString field)
{
    field_ = new Field(field);
    field_->initFieldDraw();
}

void Client::initField(QString field, QString fieldState)
{
    field_ = new Field(field, fieldState);
    field_->initFieldDraw();
}

QString Client::getFieldStr()
{
    if (!field_)
        return NULL;

    return field_->getFieldStr();
}

bool Client::isCellEmpty(int x, int y)
{
    qDebug() << "Client::isCellEmpty";
    return field_->isCellEmpty(x, y);
}

bool Client::isKilled(int x, int y)
{
    return field_->isKilled(x, y);
}

void Client::setCellState(int x, int y, Field::CellState state)
{
    return field_->setCellState(x, y, state);
}

void Client::setCellDraw(int x, int y, Field::CellDraw state)
{
    return field_->setCellDraw(x, y, state);
}

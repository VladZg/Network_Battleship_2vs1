#ifndef IMAGES_H
#define IMAGES_H

#include <QMap>
#include <QImage>
#include <QString>
#include <QDebug>
#include "config.hpp"
#include "constants.hpp"

class Images
{
public:
    Images();
    ~Images();

    void load();
    QImage& get(const QString& imgName);
    bool isLoaded();

private:
    QMap<QString, QImage> images_;
    bool isLoaded_;
};

extern Images pictures;

#endif // IMAGES_H

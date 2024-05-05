#include "images.hpp"
#include <QPixmap>

Images pictures = Images();

Images::Images():
    isLoaded_(false)
{

}

Images::~Images()
{

}

#define LOAD_IMAGE_PNG(name_str) images_.insert(name_str, QImage(":/" IMAGES_DIRECTORY_PATH name_str ".png")); \
                                 qDebug() << name_str ".png is loaded";

void Images::load()
{
    if( isLoaded_ )
        return;

    LOAD_IMAGE_PNG("background")
    LOAD_IMAGE_PNG("dot"       )
    LOAD_IMAGE_PNG("live"      )
    LOAD_IMAGE_PNG("part"      )
    LOAD_IMAGE_PNG("killed"    )
    LOAD_IMAGE_PNG("mark"      )
    LOAD_IMAGE_PNG("damaged"   )
    LOAD_IMAGE_PNG("flag"      )
//    LOAD_IMAGE_PNG("about"  )

    QMap<QString, QImage>::const_iterator imgIt = images_.find("flag");
    QPixmap pixmap = QPixmap::fromImage(imgIt.value());
    QPixmap scaledPixmap = pixmap.scaled(QSize(FIELD_IMG_WIDTH_DEFAULT/FIELD_WIDTH_DEFAULT-2, FIELD_IMG_HEIGHT_DEFAULT/FIELD_HEIGHT_DEFAULT-2), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage scaledImg = scaledPixmap.toImage();
    images_.insert(imgIt.key(), scaledImg);

    isLoaded_ = true;
}

#undef LOAD_IMAGE_PNG

QImage& Images::get(const QString& imgName)
{
    QMap<QString, QImage>::iterator img_it = images_.find(imgName);

    if(img_it == images_.end())
        throw 1;

    return img_it.value();
}

bool Images::isLoaded()
{
    return isLoaded_;
}

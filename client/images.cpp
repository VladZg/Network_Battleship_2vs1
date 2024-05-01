#include "images.h"

Images pictures = Images();

Images::Images():
    isLoaded_( false )
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
    LOAD_IMAGE_PNG("kill"      )
    LOAD_IMAGE_PNG("part"      )
    LOAD_IMAGE_PNG("redhalf"   )
    LOAD_IMAGE_PNG("mark"   )
//    LOAD_IMAGE_PNG("about"  )

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

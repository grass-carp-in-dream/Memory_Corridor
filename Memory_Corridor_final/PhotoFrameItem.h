#ifndef PHOTOFRAMEITEM_H
#define PHOTOFRAMEITEM_H
#include <QGraphicsPixmapItem>
class PhotoFrameItem : public QGraphicsPixmapItem
{
public:
    PhotoFrameItem(const QPixmap &pixmap, const QString &path)
        : QGraphicsPixmapItem(pixmap), imagePath(path) {}

    QString getImagePath() const { return imagePath; }

private:
    QString imagePath;  // 存储图片路径
};

#endif // PHOTOFRAMEITEM_H

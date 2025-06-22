#ifndef PHOTOFRAMEDATA_H
#define PHOTOFRAMEDATA_H

#include <QString>
#include <QDateTime>
#include <QDate>

struct PhotoFrameData {
    QDateTime creationTime;
    QDate date;
    QString description;
    QString imagePath;

    // 添加 getter 函数
    QString getCreationTime() const {
        return creationTime.toString(Qt::ISODate);  // 转换为字符串格式
    }

    QString getDate() const {
        return date.toString(Qt::ISODate);  // 转换为字符串格式
    }

    QString getDescription() const {
        return description;
    }

    QString getImagePath() const {
        return imagePath;
    }
};

#endif // PHOTOFRAMEDATA_H

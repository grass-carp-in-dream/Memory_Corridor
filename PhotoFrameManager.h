#ifndef PHOTOFRAMEMANAGER_H
#define PHOTOFRAMEMANAGER_H

#include "PhotoFrameData.h"
#include <QVector>
#include <QString>
#include <QDateTime>

class PhotoFrameManager {
public:
    PhotoFrameManager() = default;

    // 加载相框数据从文件
    bool loadFromFile(const QString &filePath);

    // 获取所有相框数据
    const QVector<PhotoFrameData>& getAllFrames() const;

    // 新增：保存相框数据到文件
    bool saveToFile(const QString &filePath) const;
    bool isImportantSingleChar(const QString &word) const;

    struct YearlySummary{
        int totalFrames;
        int framesPerMonth[12];  //，每个月的相框数
        QMap<QString, int > keywordCounts;  //描述中的关键词
        QVector<QString> mostUsedImages;    //最常用的图片路径
    };

    YearlySummary getYearlySummary(int year) const;

private:
    QVector<PhotoFrameData> m_frames;  // 存储所有相框数据
};

#endif // PHOTOFRAMEMANAGER_H

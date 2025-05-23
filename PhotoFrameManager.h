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

private:
    QVector<PhotoFrameData> m_frames;  // 存储所有相框数据
};

#endif // PHOTOFRAMEMANAGER_H

#include "PhotoFrameManager.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

bool PhotoFrameManager::loadFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file:" << filePath;
        return false;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
        return false;
    }

    if (!doc.isArray()) {
        qWarning() << "JSON is not an array!";
        return false;
    }

    QJsonArray arr = doc.array();
    m_frames.clear();

    for (const QJsonValue &val : arr) {
        if (!val.isObject())
            continue;

        QJsonObject obj = val.toObject();
        PhotoFrameData frame;

        frame.creationTime = QDateTime::fromString(obj.value("creationTime").toString(), Qt::ISODate);
        frame.date = QDate::fromString(obj.value("date").toString(), Qt::ISODate);
        frame.description = obj.value("description").toString();
        frame.imagePath = obj.value("imagePath").toString();

        m_frames.append(frame);
    }

    return true;
}

bool PhotoFrameManager::saveToFile(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open file for writing:" << filePath;
        return false;
    }

    QJsonArray frameArray;

    for (const PhotoFrameData &frame : m_frames) {
        QJsonObject frameObj;
        frameObj["creationTime"] = frame.creationTime.toString(Qt::ISODate);
        frameObj["date"] = frame.date.toString(Qt::ISODate);
        frameObj["description"] = frame.description;
        frameObj["imagePath"] = frame.imagePath;

        frameArray.append(frameObj);
    }

    QJsonDocument doc(frameArray);
    file.write(doc.toJson());
    file.close();

    return true;
}

const QVector<PhotoFrameData>& PhotoFrameManager::getAllFrames() const
{
    return m_frames;
}

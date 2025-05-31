#ifndef PHOTOFRAMEWIDGET_H
#define PHOTOFRAMEWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include "ClickableLabel.h"
#include<QDate>
#include <QJsonObject>

class PhotoFrameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PhotoFrameWidget(QWidget *parent = nullptr);

    void setImage(const QPixmap &pix);
    void setImage(const QString &path);
    QPixmap image() const;

    void setDescription(const QString &text);
    QString description() const;

    void setDate(const QDate &date);
    QDate getDate() const;
    QDateTime getTimestamp() const; // 包含创建时间，用于精细排序
    QString getDescription() const;

    QJsonObject toJson() const;
    void fromJson(const QJsonObject &obj);

signals:
    void requestDelete(PhotoFrameWidget* self);
    void requestUpdate();

private slots:
    void onImageClicked(Qt::MouseButton button);

private:
    void showImageViewer();
    void askDeleteConfirmation();

    ClickableLabel *imageLabel;
    QLineEdit *descriptionEdit;
    QString imagePath; // 原始图片路径，用于查看原图
    QLabel *dateLabel; // 记录日期

    QDate date;
    QDateTime creationTime;

    QSize fixedSize = QSize(150, 150);  // 默认150x150的大小
    int margin = 10;  // 设置边距，间距为10px
protected:
    void mouseDoubleClickEvent(QMouseEvent *event) override;

};

#endif // PHOTOFRAMEWIDGET_H

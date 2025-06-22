#ifndef ALBUMPAGE_H
#define ALBUMPAGE_H

#include <QWidget>
#include <QPushButton>
#include "photoframewidget.h"
#include "FlowLayout.h"
#include <QScrollArea>
#include <QFile>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDir>


class AlbumPage : public QWidget
{
    Q_OBJECT
public:
    explicit AlbumPage(QWidget *parent = nullptr);
    void saveAlbumToFile(const QString &filePath);
    void loadAlbumFromFile(const QString &filePath);

signals:
    void backToMainRequested();
    void backToGameRequested();  // 返回游戏页面

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void onAddPhoto();
    void onRemovePhoto(PhotoFrameWidget *frame);
    void onBackButtonClicked();
    void onSaveButtonClicked();
    void onBackToGameButtonClicked();
    void onLoadAlbum();

private:
    FlowLayout *flowLayout;
    QWidget *buttonWidget;
    QPushButton *addButton;
    QPushButton *backButton;
    QPushButton *saveButton;
    QPushButton *loadButton;
    QPushButton *backToGameButton;
    QScrollArea *scrollArea;
    QWidget *scrollContent;

    QList<PhotoFrameWidget*> photoFrames;

    void adjustPhotoFrameSizes();
    void createButtons();
    QScrollArea* createFlowLayout();
    void sortPhotoFrames();
};

#endif // ALBUMPAGE_H

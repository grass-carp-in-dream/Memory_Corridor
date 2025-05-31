#include "backgroundsettingspage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QFileDialog>
#include <QPixmap>
#include <QDebug>

BackgroundSettingsPage::BackgroundSettingsPage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("背景设置", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    previewLabel = new QLabel("当前无预览", this);
    previewLabel->setFixedSize(300, 180);
    previewLabel->setStyleSheet("border: 1px solid gray; background: #eee;");
    previewLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(previewLabel);

    // 默认背景路径（你可以改为自己的资源路径）
    defaultImagePaths = {
        ":/new/prefix1/background1.png",
        ":/new/prefix1/background2.png",
        ":/new/prefix1/background3.png",
        ":/new/prefix1/background4.png",
        ":/new/prefix1/background5.png",
        ":/new/prefix1/background6.png",
        ":/new/prefix1/background7.png",
        ":/new/prefix1/background8.png"
    };

    QGridLayout *grid = new QGridLayout();
    for (int i = 0; i < defaultImagePaths.size(); ++i) {
        QPushButton *btn = new QPushButton(this);
        btn->setFixedSize(70, 50);
        QPixmap pix(defaultImagePaths[i]);
        btn->setIcon(pix);
        btn->setIconSize(btn->size());
        btn->setProperty("imagePath", defaultImagePaths[i]);
        connect(btn, &QPushButton::clicked, this, &BackgroundSettingsPage::onDefaultImageClicked);
        grid->addWidget(btn, i / 4, i % 4);
        defaultButtons.append(btn);
    }
    layout->addLayout(grid);

    chooseButton = new QPushButton("选择自定义背景图片", this);
    layout->addWidget(chooseButton);
    layout->addStretch();

    connect(chooseButton, &QPushButton::clicked, this, &BackgroundSettingsPage::onChooseImage);
}

void BackgroundSettingsPage::onDefaultImageClicked()
{
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString path = btn->property("imagePath").toString();
    QPixmap pix(path);
    if (!pix.isNull()) {
        previewLabel->setPixmap(pix.scaled(previewLabel->size(), Qt::KeepAspectRatio));
    }

    qDebug() << "Default background selected:" << path;
    emit backgroundImageChanged(path);
}

void BackgroundSettingsPage::onChooseImage()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择背景图片"),
                                                    QString(),
                                                    tr("图片文件 (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap pix(fileName);
        if (!pix.isNull()) {
            previewLabel->setPixmap(pix.scaled(previewLabel->size(), Qt::KeepAspectRatio));
        }

        qDebug() << "Custom background selected:" << fileName;
        emit backgroundImageChanged(fileName);
    }
}

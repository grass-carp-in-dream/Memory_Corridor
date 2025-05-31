#include "charactersettingspage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QSlider>
#include <QCloseEvent>

CharacterSettingsPage::CharacterSettingsPage(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    QLabel *titleLabel = new QLabel("角色设置", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // 角色形象选择
    QLabel *imageLabel = new QLabel("选择角色形象：", this);
    imageComboBox = new QComboBox(this);
    imageComboBox->addItem("角色1", ":/new/prefix1/Girl_1.png");
    imageComboBox->addItem("角色2", ":/new/prefix1/Girl_2.png");
    imageComboBox->addItem("角色3", ":/new/prefix1/Girl_3.png");
    imageComboBox->addItem("角色4", ":/new/prefix1/Boy_1.png");
    imageComboBox->addItem("角色5", ":/new/prefix1/Boy_2.png");
    imageComboBox->addItem("角色6", ":/new/prefix1/Boy_3.png");


    QHBoxLayout *imageLayout = new QHBoxLayout();
    imageLayout->addWidget(imageLabel);
    imageLayout->addWidget(imageComboBox);
    mainLayout->addLayout(imageLayout);

    // 角色大小调整
    QLabel *sizeLabel = new QLabel("角色大小（缩放比例）：", this);
    sizeSlider = new QSlider(Qt::Horizontal, this);
    sizeSlider->setRange(50, 200);  // 0.5 到 2.0 倍
    sizeSlider->setValue(100);       // 默认 1.0 倍

    QHBoxLayout *sizeLayout = new QHBoxLayout();
    sizeLayout->addWidget(sizeLabel);
    sizeLayout->addWidget(sizeSlider);
    mainLayout->addLayout(sizeLayout);

    // 角色Y轴偏移调整
    QLabel *yOffsetLabel = new QLabel("角色Y偏移量：", this);
    yOffsetSlider = new QSlider(Qt::Horizontal, this);
    yOffsetSlider->setRange(-100, 100);
    yOffsetSlider->setValue(0);

    QHBoxLayout *yOffsetLayout = new QHBoxLayout();
    yOffsetLayout->addWidget(yOffsetLabel);
    yOffsetLayout->addWidget(yOffsetSlider);
    mainLayout->addLayout(yOffsetLayout);

    // 连接信号槽
    connect(imageComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &CharacterSettingsPage::onImageChanged);
    connect(sizeSlider, &QSlider::valueChanged,
            this, &CharacterSettingsPage::onSizeChanged);
    connect(yOffsetSlider, &QSlider::valueChanged,
            this, &CharacterSettingsPage::onYOffsetChanged);

    // 初始化时发射信号，确保外部能拿到默认值
    onImageChanged(imageComboBox->currentIndex());
    onSizeChanged(sizeSlider->value());
    onYOffsetChanged(yOffsetSlider->value());
}

void CharacterSettingsPage::onImageChanged(int index)
{
    QString imagePath = imageComboBox->itemData(index).toString();
    qDebug().noquote() << Q_FUNC_INFO
                       << "\n  index =" << index
                       << "\n  imagePath =" << imagePath;
    emit characterImageChanged(imagePath);
}

void CharacterSettingsPage::onSizeChanged(int value)
{
    double scale = value / 100.0;
    qDebug().noquote() << Q_FUNC_INFO
                       << "\n  slider value =" << value
                       << "\n  scale =" << scale;
    emit characterSizeChanged(scale);
}

void CharacterSettingsPage::onYOffsetChanged(int value)
{
    qDebug().noquote() << Q_FUNC_INFO
                       << "\n  yOffset value =" << value;
    emit characterYOffsetChanged(value);
}

void CharacterSettingsPage::closeEvent(QCloseEvent *event)
{
    qDebug() << Q_FUNC_INFO << "Closed";
    emit closed();
    QWidget::closeEvent(event);
}

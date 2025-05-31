#include "musicsettingspage.h"
#include <QVBoxLayout>
#include <QComboBox>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>

MusicSettingsPage::MusicSettingsPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *trackLabel = new QLabel("选择背景音乐：");
    trackComboBox = new QComboBox(this);
    trackComboBox->addItems({
        "BGM1 - 甜美的微笑",
        "BGM2 - 蒙德的一日",
        "BGM3 - 风所爱之城"
    });

    QLabel *volumeLabel = new QLabel("音量调节：");
    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);
    volumeSlider->setValue(50);

    muteCheckBox = new QCheckBox("静音", this);

    layout->addWidget(trackLabel);
    layout->addWidget(trackComboBox);
    layout->addWidget(volumeLabel);
    layout->addWidget(volumeSlider);
    layout->addWidget(muteCheckBox);
    layout->addStretch();

    // 信号连接
    connect(trackComboBox, &QComboBox::currentTextChanged, this, [=](const QString &name) {
        emit musicTrackChanged(getTrackPathByName(name));
    });
    connect(volumeSlider, &QSlider::valueChanged, this, &MusicSettingsPage::musicVolumeChanged);
    connect(muteCheckBox, &QCheckBox::toggled, this, &MusicSettingsPage::musicMutedChanged);
}

// 返回本地路径
QString MusicSettingsPage::getTrackPathByName(const QString &name)
{
    static QMap<QString, QString> trackMap = {
        // 这里的文件路径要改成你们自己保存的音乐文件的绝对路径
        {"BGM1 - 甜美的微笑", "D:/程设大作业/Memory_Corridor/甜美的微笑.mp3"},
        {"BGM2 - 蒙德的一日", "D:/程设大作业/Memory_Corridor/蒙德的一日.mp3"},
        {"BGM3 - 风所爱之城", "D:/程设大作业/Memory_Corridor/风所爱之城.mp3"}
    };
    return trackMap.value(name, "");
}


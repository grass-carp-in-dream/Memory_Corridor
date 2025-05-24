#include "gamesettingspage.h"

GameSettingsPage::GameSettingsPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    speedLabel = new QLabel("角色移动速度：50", this);
    speedSlider = new QSlider(Qt::Horizontal, this);
    speedSlider->setRange(10, 100);  // 自定义速度范围
    speedSlider->setValue(50);

    layout->addWidget(speedLabel);
    layout->addWidget(speedSlider);

    connect(speedSlider, &QSlider::valueChanged,
            this, &GameSettingsPage::onSpeedSliderChanged);
}

void GameSettingsPage::onSpeedSliderChanged(int value)
{
    speedLabel->setText(QString("角色移动速度：%1").arg(value));
    emit characterSpeedChanged(value);
}

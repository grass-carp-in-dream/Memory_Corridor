#include "settingspage.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QCheckBox>
#include <QFileDialog>
#include <QDebug>
#include "backgroundsettingspage.h"


SettingsPage::SettingsPage(QWidget *parent) : QWidget(parent)
{
    setupUi();
    this->resize(800, 600);
}

void SettingsPage::setupUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    tabWidget = new QTabWidget(this);

    setupBackgroundSettings();
    setupSoundSettings();
    setupGameSettings();
    setupCharacterSettings();
    setupPetSettings();

    tabWidget->addTab(backgroundSettingsPage, tr("背景设置"));
    tabWidget->addTab(soundSettingsPage, tr("声音设置"));
    tabWidget->addTab(gameSettingsPage, tr("游戏设置"));
    tabWidget->addTab(characterSettingsPage, tr("角色设置"));  // 嵌入角色设置页面
    tabWidget->addTab(petSettingsPage, tr("桌宠设置"));

    mainLayout->addWidget(tabWidget);
}

void SettingsPage::setupBackgroundSettings()
{
    backgroundSettingsPage = new BackgroundSettingsPage(this);

    connect(static_cast<BackgroundSettingsPage*>(backgroundSettingsPage),
            &BackgroundSettingsPage::backgroundImageChanged,
            this, &SettingsPage::onBackgroundImageChanged);
}


void SettingsPage::setupSoundSettings()
{
    soundSettingsPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(soundSettingsPage);

    QLabel *bgmLabel = new QLabel(tr("背景音乐音量"));
    QSlider *bgmSlider = new QSlider(Qt::Horizontal);
    bgmSlider->setRange(0, 100);
    bgmSlider->setValue(50);

    QLabel *effectLabel = new QLabel(tr("音效音量"));
    QSlider *effectSlider = new QSlider(Qt::Horizontal);
    effectSlider->setRange(0, 100);
    effectSlider->setValue(50);

    QCheckBox *muteCheckbox = new QCheckBox(tr("静音"));

    layout->addWidget(bgmLabel);
    layout->addWidget(bgmSlider);
    layout->addWidget(effectLabel);
    layout->addWidget(effectSlider);
    layout->addWidget(muteCheckbox);
    layout->addStretch();

    // TODO: 连接滑块和复选框信号，调整音量或静音状态
}

void SettingsPage::setupGameSettings()
{
    gameSettingsPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(gameSettingsPage);

    QLabel *speedLabel = new QLabel(tr("角色移动速度"));
    QSlider *speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(1, 10);
    speedSlider->setValue(5);

    layout->addWidget(speedLabel);
    layout->addWidget(speedSlider);
    layout->addStretch();

    // TODO: 连接滑块信号，调整游戏内角色速度设置
}

void SettingsPage::setupCharacterSettings()
{
    characterSettingsPage = new CharacterSettingsPage(this);

    // 信号转发
    connect(characterSettingsPage, &CharacterSettingsPage::characterImageChanged,
            this, &SettingsPage::onCharacterImageChanged);
    connect(characterSettingsPage, &CharacterSettingsPage::characterSizeChanged,
            this, &SettingsPage::onCharacterScaleChanged);
    connect(characterSettingsPage, &CharacterSettingsPage::characterYOffsetChanged,
            this, &SettingsPage::onCharacterYOffsetChanged);
    tabWidget->addTab(characterSettingsPage, "角色设置");
}

void SettingsPage::setupPetSettings()
{
    petSettingsPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(petSettingsPage);

    QLabel *label = new QLabel(tr("桌宠设置（示例）"));
    QPushButton *choosePetButton = new QPushButton(tr("选择桌宠图片"));

    layout->addWidget(label);
    layout->addWidget(choosePetButton);
    layout->addStretch();

    connect(choosePetButton, &QPushButton::clicked, this, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, tr("选择桌宠图片"),
                                                        QString(),
                                                        tr("图片文件 (*.png *.jpg *.bmp)"));
        if (!fileName.isEmpty()) {
            qDebug() << "选择桌宠图片路径：" << fileName;
            // TODO: 发送信号通知主窗口或游戏页面切换桌宠
        }
    });
}
//角色设置槽函数
void SettingsPage::onCharacterImageChanged(const QString &path)
{
    qDebug() << "SettingsPage: Character Image Changed to " << path;
    emit characterImageChanged(path);  // 发射信号
}

void SettingsPage::onCharacterScaleChanged(double scale)
{
    qDebug() << "SettingsPage: Character Scale Changed to " << scale;
    emit characterScaleChanged(scale);  // 发射信号
}

void SettingsPage::onCharacterYOffsetChanged(int offset)
{
    qDebug() << "SettingsPage: Character Y Offset Changed to " << offset;
    emit characterYOffsetChanged(offset);  // 发射信号
}

void SettingsPage::onBackgroundImageChanged(const QString &path)
{
    qDebug() << "SettingsPage: Background Image Changed to " << path;
    emit backgroundImageChanged(path);  // 转发信号给主窗口或游戏页
}

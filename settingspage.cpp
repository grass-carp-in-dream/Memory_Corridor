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
    tabWidget->addTab(musicSettingsPage, tr("声音设置"));
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
    musicSettingsPage = new MusicSettingsPage(this);

    // 接收 MusicSettingsPage 的信号，转发给外部
    connect(musicSettingsPage, &MusicSettingsPage::musicTrackChanged, this, [=](const QString &path) {
        qDebug() << "[SettingsPage] 转发音乐路径：" << path;
        emit bgmTrackChanged(path);  // 转发信号
    });

    connect(musicSettingsPage, &MusicSettingsPage::musicVolumeChanged, this, &SettingsPage::bgmVolumeChanged);
    connect(musicSettingsPage, &MusicSettingsPage::musicMutedChanged, this, &SettingsPage::muteToggled);
}


void SettingsPage::setupGameSettings()
{
    gameSettingsPage = new GameSettingsPage(this);
    connect(gameSettingsPage, &GameSettingsPage::characterSpeedChanged,
            this, &SettingsPage::characterSpeedChanged);  // 转发信号
    tabWidget->addTab(gameSettingsPage, "游戏设置");
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

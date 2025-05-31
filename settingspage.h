#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QTabWidget>
#include "charactersettingspage.h"  // 引入角色设置界面
#include "backgroundsettingspage.h" // 引入背景设置界面
#include "musicsettingspage.h"      // 引入音乐设置界面
#include "gamesettingspage.h"       // 引入游戏设置界面


class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);

signals:
    // 角色设置信号
    void characterImageChanged(const QString &path);
    void characterScaleChanged(double scale);
    void characterYOffsetChanged(int offset);
    //背景设置信号
    void backgroundImageChanged(const QString &path);
    //音乐设置信号
    void bgmVolumeChanged(int volume);   // 背景音乐音量变化
    void muteToggled(bool muted);        // 静音状态变化
    void bgmTrackChanged(const QString &path);  // 设置页面发出的信号
    // 游戏设置信号
    void characterSpeedChanged(int speed);



private slots:
    // 角色设置槽函数
    void onCharacterImageChanged(const QString &path);
    void onCharacterScaleChanged(double scale);
    void onCharacterYOffsetChanged(int offset);
    void onBackgroundImageChanged(const QString &path);

private:
    QTabWidget *tabWidget;

    BackgroundSettingsPage *backgroundSettingsPage;
    MusicSettingsPage *musicSettingsPage;
    GameSettingsPage *gameSettingsPage;
    CharacterSettingsPage *characterSettingsPage;
    QWidget *petSettingsPage;

    void setupUi();
    void setupBackgroundSettings();
    void setupSoundSettings();
    void setupGameSettings();
    void setupCharacterSettings();
    void setupPetSettings();
};

#endif // SETTINGSPAGE_H

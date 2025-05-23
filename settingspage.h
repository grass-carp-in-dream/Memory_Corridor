#ifndef SETTINGSPAGE_H
#define SETTINGSPAGE_H

#include <QWidget>
#include <QTabWidget>
#include "charactersettingspage.h"  // 引入角色设置界面
#include "backgroundsettingspage.h" // 引入背景设置界面


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
    void backgroundImageChanged(const QString &path);


private slots:
    // 角色设置槽函数
    void onCharacterImageChanged(const QString &path);
    void onCharacterScaleChanged(double scale);
    void onCharacterYOffsetChanged(int offset);
    void onBackgroundImageChanged(const QString &path);

private:
    QTabWidget *tabWidget;

    BackgroundSettingsPage *backgroundSettingsPage;
    QWidget *soundSettingsPage;
    QWidget *gameSettingsPage;
    CharacterSettingsPage *characterSettingsPage;  // 角色设置页面指针
    QWidget *petSettingsPage;

    void setupUi();
    void setupBackgroundSettings();
    void setupSoundSettings();
    void setupGameSettings();
    void setupCharacterSettings();
    void setupPetSettings();
};

#endif // SETTINGSPAGE_H

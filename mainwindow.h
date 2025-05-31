#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include "albumpage.h"
#include "PhotoFrameManager.h"
#include "gamepage.h"
#include "charactersettingspage.h"
#include "settingspage.h"
#include <QMediaPlayer>
#include <QAudioOutput>

/*
 * MainWindow 是整个程序的主窗口类，包含：
 * - 主菜单页 mainMenuPage （作为 stackedWidget 的页面）
 * - 相册页面 albumPage （stackedWidget 页面）
 * - 游戏页面 gamePage （stackedWidget 页面）
 *
 * settingsPage 和 characterSettingsPage 不作为 stackedWidget 的页面，而是独立的窗口弹出管理
 *
 * 主要界面控件：
 * - 各种按钮、标签，控制页面切换和功能触发
 *
 * 其他：
 * - PhotoFrameManager 用于管理相框数据（调试用）
 * - normalWindowSize 用来记录窗口非全屏时的大小，方便切换
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override; // 重写resize事件，动态调整字体大小等

public slots:
    void showSettingsPage();         // 弹出设置窗口槽函数
    //角色设置信号
    void onCharacterImageChanged(const QString &imagePath);
    void onCharacterScaleChanged(double scale);
    void onCharacterYOffsetChanged(int offset);
    //背景设置信号
    void onBackgroundImageChanged(const QString &path);
    //音乐设置信号
    void onBgmTrackChanged(const QString &path);
    void onBgmVolumeChanged(int volume);
    void onBgmMuteToggled(bool muted);
    //游戏设置信号
    void onCharacterSpeedChanged(int speed);
    void showYearlyReport();
private:
    QStackedWidget *stackedWidget;   // 管理主菜单、相册、游戏页切换
    QWidget *mainMenuPage;            // 主菜单页面

    AlbumPage *albumPage;             // 相册页面
    GamePage *gamePage;               // 游戏页面（stackedWidget）

    SettingsPage *settingsPage;       // 设置页面（独立窗口，不加入 stackedWidget）

    // 主菜单界面控件
    QLabel *themeLabel;               // 标题标签
    QPushButton *startButton;        // 开始游戏按钮
    QPushButton *albumButton;        // 进入相册按钮
    QPushButton *settingsButton;     // 打开设置按钮
    QPushButton *summaryButton;      // 总结报告按钮

    PhotoFrameManager m_photoFrameManager; // 相框管理（调试用）
    QSize normalWindowSize;           // 记录窗口非全屏大小，方便切换
    QMediaPlayer *bgmPlayer = nullptr;  // 音乐播放器
    QAudioOutput* bgmAudioOutput = nullptr;// 音量设置器


};

#endif // MAINWINDOW_H

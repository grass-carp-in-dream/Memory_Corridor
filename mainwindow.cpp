#include "mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QFont>
#include <QResizeEvent>
#include <QDebug>
#include <QMessageBox>
#include "albumpage.h"
#include "settingspage.h"
#include "PhotoFrameManager.h"
#include "yearlyreportdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(400, 300);
    normalWindowSize = size();


    // 创建堆叠窗口，作为主窗口的中央部件
    stackedWidget = new QStackedWidget(this);
    this->setCentralWidget(stackedWidget);

    // 主菜单页面及其布局和控件
    mainMenuPage = new QWidget(this);
    QVBoxLayout *menuLayout = new QVBoxLayout(mainMenuPage);
    menuLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    themeLabel = new QLabel("回忆长廊");
    themeLabel->setAlignment(Qt::AlignCenter);

    startButton = new QPushButton("开始");
    albumButton = new QPushButton("相册");
    settingsButton = new QPushButton("设置");
    summaryButton = new QPushButton("总结报告");

    menuLayout->addWidget(themeLabel);
    menuLayout->addSpacing(30);
    menuLayout->addWidget(startButton);
    menuLayout->addWidget(albumButton);
    menuLayout->addWidget(settingsButton);
    menuLayout->addWidget(summaryButton);

    // 相册页面和游戏页面
    albumPage = new AlbumPage(this);
    gamePage = new GamePage(this);

    // 设置页面指针初始化为空，延迟创建
    settingsPage = nullptr;

    // 将页面加入堆叠控件
    stackedWidget->addWidget(mainMenuPage);  // index 0
    stackedWidget->addWidget(albumPage);     // index 1
    stackedWidget->addWidget(gamePage);      // index 2
    stackedWidget->setCurrentIndex(0);

    // 按钮信号连接，切换页面或打开窗口
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::showSettingsPage);
    connect(albumButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(1); });
    connect(albumPage, &AlbumPage::backToMainRequested, this, [=]() { stackedWidget->setCurrentIndex(0); });
    connect(startButton, &QPushButton::clicked, this, [=]() { stackedWidget->setCurrentIndex(2); });
    connect(summaryButton, &QPushButton::clicked, this, &MainWindow::showYearlyReport);

    // 切换页面时处理窗口大小和焦点
    connect(stackedWidget, &QStackedWidget::currentChanged, this, [=](int index) {
        if (index == 2) {
            this->showFullScreen();
            gamePage->setFocusToGameView();
        } else {
            this->showNormal();
            this->resize(normalWindowSize);
        }
    });

    connect(gamePage, &GamePage::backToMainRequested, this, [=]() { stackedWidget->setCurrentIndex(0); });
    connect(gamePage, &GamePage::showSettingsPage, this, &MainWindow::showSettingsPage);
    // 主窗口的构造函数中，连接 GamePage 的信号
    connect(albumPage, &AlbumPage::backToGameRequested, this,  [=]() { stackedWidget->setCurrentIndex(2); });


    // 加载相框数据
    // 加载相框数据
    // 这里的文件路径要改成你们自己保存的json文件的绝对路径
    bool success = m_photoFrameManager.loadFromFile("C:\\Users\\34893\\Desktop\\test_memory_corridor_data.json");

    if (success) {
        qDebug() << "Photo frames loaded successfully!";
        gamePage->loadAndDisplayFrames();
        // 获取所有相框数据
        const QVector<PhotoFrameData>& photoFrames = m_photoFrameManager.getAllFrames();

        // 遍历并输出每个相框的数据
        for (const PhotoFrameData &frame : photoFrames) {
            qDebug() << "Creation Time:" << frame.getCreationTime()
            << "Date:" << frame.getDate()
            << "Description:" << frame.getDescription()
            << "Image Path:" << frame.getImagePath();
        }
    } else {
        qDebug() << "Failed to load photo frames.";
    }
    // 播放默认BGM
    // 这里的文件路径要改成你们自己保存的音乐文件的绝对路径
    QString defaultBgmPath = "C:/Users/34893/Desktop/Memory_Corridor/Memory_corridor/甜美的微笑.mp3";
    onBgmTrackChanged(defaultBgmPath);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    int w = event->size().width();

    QFont titleFont;
    titleFont.setPointSize(std::max(w / 15, 14));

    QFont btnFont;
    btnFont.setPointSize(std::max(w / 25, 12));

    themeLabel->setFont(titleFont);
    startButton->setFont(btnFont);
    albumButton->setFont(btnFont);
    settingsButton->setFont(btnFont);
    summaryButton->setFont(btnFont);
}

void MainWindow::showSettingsPage()
{
    if (!settingsPage) {
        settingsPage = new SettingsPage(this);
        settingsPage->setWindowModality(Qt::ApplicationModal);
        settingsPage->setWindowFlag(Qt::Window);

        // 之前的信号连接...
        connect(settingsPage, &SettingsPage::characterImageChanged, this, &MainWindow::onCharacterImageChanged);
        connect(settingsPage, &SettingsPage::characterScaleChanged, this, &MainWindow::onCharacterScaleChanged);
        connect(settingsPage, &SettingsPage::characterYOffsetChanged, this, &MainWindow::onCharacterYOffsetChanged);
        connect(settingsPage, &SettingsPage::backgroundImageChanged, this, &MainWindow::onBackgroundImageChanged);
        connect(settingsPage, &SettingsPage::bgmTrackChanged, this, &MainWindow::onBgmTrackChanged);
        connect(settingsPage, &SettingsPage::bgmVolumeChanged, this, &MainWindow::onBgmVolumeChanged);
        connect(settingsPage, &SettingsPage::muteToggled, this, &MainWindow::onBgmMuteToggled);

        // 新增角色速度信号连接
        connect(settingsPage, &SettingsPage::characterSpeedChanged,
                this, &MainWindow::onCharacterSpeedChanged);
    }

    settingsPage->show();
    settingsPage->raise();
    settingsPage->activateWindow();
}

void MainWindow::showYearlyReport()
{
    qDebug() << "Attempting to show yearly report";

    if (m_photoFrameManager.getAllFrames().isEmpty()) {
        qDebug() << "No frames available";
        QMessageBox::warning(this, "警告", "没有可用的相框数据！");
        return;
    }

    qDebug() << "Creating dialog with" << m_photoFrameManager.getAllFrames().size() << "frames";
    YearlyReportDialog *dialog = new YearlyReportDialog(m_photoFrameManager, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose); // 自动删除
    dialog->show(); // 使用 show() 而不是 exec() 测试
    dialog->raise();
    dialog->activateWindow();
}


void MainWindow::onCharacterImageChanged(const QString &imagePath)
{
    if (gamePage) {
        gamePage->setCharacterImage(imagePath); // 将信号转发到 gamePage
    }
}

void MainWindow::onCharacterScaleChanged(double scale)
{
    if (gamePage) {
        gamePage->setCharacterScale(scale); // 将信号转发到 gamePage
    }
}

void MainWindow::onCharacterYOffsetChanged(int offset)
{
    qDebug() << "[MainWindow] onCharacterYOffsetChanged called:" << offset;
    if (gamePage) {
        gamePage->setCharacterYOffset(offset); // 将信号转发到 gamePage
    }
}

void MainWindow::onBackgroundImageChanged(const QString &path)
{
    qDebug() << "[MainWindow] Background Image Changed to" << path;
    if (gamePage) {
        gamePage->setBackgroundImage(path);
    }
}

void MainWindow::onBgmTrackChanged(const QString &path)
{
    qDebug() << "[MainWindow] 播放新的音乐：" << path;

    if (!bgmPlayer) {
        bgmAudioOutput = new QAudioOutput(this);
        bgmPlayer = new QMediaPlayer(this);
        bgmPlayer->setAudioOutput(bgmAudioOutput);
        bgmPlayer->setLoops(QMediaPlayer::Infinite);  // 设置为无限循环播放
    } else {
        bgmPlayer->stop();  // 停止当前音乐（可选，避免冲突）
        bgmPlayer->setLoops(QMediaPlayer::Infinite);  // 再次设置循环（确保切歌后生效）
    }

    bgmPlayer->setSource(QUrl::fromLocalFile(path));
    bgmPlayer->play();
}



void MainWindow::onBgmVolumeChanged(int volume)
{
    if (bgmAudioOutput) {
        qDebug() << "[MainWindow] 设置音量：" << volume;
        bgmAudioOutput->setVolume(volume / 100.0);  // 注意 Qt 6 音量是 0.0 ~ 1.0
    }
}


void MainWindow::onBgmMuteToggled(bool muted)
{
    if (bgmAudioOutput) {
        qDebug() << "[MainWindow] 静音状态：" << muted;
        bgmAudioOutput->setMuted(muted);
    }
}

void MainWindow::onCharacterSpeedChanged(int speed)
{
    if (gamePage) {
        gamePage->onCharacterSpeedChanged(speed);
    }
}


MainWindow::~MainWindow()
{
    // 子窗口会随父窗口自动销毁，不用特别删除
}

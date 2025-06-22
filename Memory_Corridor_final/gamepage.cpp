#include "gamepage.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "PhotoFrameManager.h"
#include "yearlyreportdialog.h"
#include "aichatdialog.h"
#include <QMovie> 

GamePage::GamePage(QWidget *parent)
    : QWidget(parent),dragging(false),chatDialog(nullptr), currentEmotion(Normal)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    //Vpet部分
    //******************************************************************* */
    //ui->setupUi(this);

    // 创建用于显示动画的 QLabel
    m_petLabel = new QLabel(this); // 我们需要一个新的成员变量 m_petLabel
    layout->addWidget(m_petLabel); // 将 Label 添加到布局
    layout->setContentsMargins(0, 0, 0, 0); // 让布局填满窗口

    // 设置窗口无边框和背景透明
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    movie = new QMovie(this);
    m_petLabel->setMovie(movie); // 将动画设置到新的 m_petLabel 上
    
    updateAnimation(); //根据初始情绪加载动画
    movie->start();
    //******************************************************************* */


    // 创建并初始化游戏视图

    gameView = new GameView(this);  // 创建游戏视图对象
    gameView->initScene();  // 初始化游戏场景
    layout->addWidget(gameView);  // 将游戏视图添加到布局中

    // 创建返回主菜单按钮
    backButton = new QPushButton("返回主菜单", this);  // 创建返回按钮
    layout->addWidget(backButton);  // 将按钮添加到布局中

    // 创建设置按钮
    settingsButton = new QPushButton("设置", this);  // 创建设置按钮
    layout->addWidget(settingsButton);  // 将设置按钮添加到布局中

    // 创建加载相框按钮
    loadButton = new QPushButton("加载相框", this);  // 创建加载按钮
    layout->addWidget(loadButton);  // 将按钮添加到布局中

    // 创建相框管理器
    frameManager = new PhotoFrameManager();

    // 连接返回按钮的点击信号到本类的槽函数
    connect(backButton, &QPushButton::clicked, this, &GamePage::backToMainRequested);

    // 连接设置按钮点击信号到显示设置页面的信号
    connect(settingsButton, &QPushButton::clicked, this, &GamePage::showSettingsPage);

    // 连接加载相框按钮的点击信号，调用加载并展示相框的函数
    connect(loadButton, &QPushButton::clicked, this, &GamePage::loadAndDisplayFrames);

    //以下是菜单创建
    //*************************** */
    contextMenu = new QMenu(this);
    QAction *reportAction = new QAction("年度报告", this);
    QAction *aiChatAction = new QAction("AI对话", this);
    
    contextMenu->addAction(reportAction);
    contextMenu->addAction(aiChatAction);

    // 连接菜单项的 triggered 信号到我们刚刚在 .h 文件中声明的槽函数
    connect(reportAction, &QAction::triggered, this, &GamePage::showYearlyReport);
    connect(aiChatAction, &QAction::triggered, this, &GamePage::openAIChatDialog);
    //*************************** */
    
    // 设置游戏视图获取焦点，方便用户进行游戏控制（例如键盘事件）
    gameView->setFocusPolicy(Qt::StrongFocus);  // 设置焦点策略
    gameView->setFocus();  // 设置焦点到游戏视图

}

void GamePage::setFocusToGameView()
{
    // 设置焦点到游戏视图，方便用户控制
    if (gameView) {
        gameView->setFocusPolicy(Qt::StrongFocus);
        gameView->setFocus();  // 获取焦点
    }
}

void GamePage::setCharacterImage(const QString &imagePath)
{
    // 设置角色图像，并将该图像传递给游戏视图
    qDebug() << "[GamePage] setCharacterImage called:" << imagePath;
    if (gameView) {
        gameView->setCharacterImage(imagePath);  // 将角色图像设置到游戏视图
        // 可根据需要缓存图片，代码示例
        // m_characterImage.load(imagePath);
    }
}

void GamePage::setCharacterScale(double scale)
{
    // 设置角色的缩放比例，并将该比例传递给游戏视图
    qDebug() << "[GamePage] setCharacterScale called:" << scale;
    m_characterScale = scale;  // 保存缩放比例
    if (gameView) {
        gameView->setCharacterScale(scale);  // 设置角色的缩放比例
    }
}

void GamePage::setCharacterYOffset(int offset)
{
    // 设置角色的垂直偏移量，并将该偏移量传递给游戏视图
    qDebug() << "[GamePage] setCharacterYOffset called:" << offset;
    m_characterYOffset = offset;  // 保存偏移量
    if (gameView) {
        gameView->setCharacterYOffset(offset);  // 设置角色的垂直偏移量
    }
}

void GamePage::loadAndDisplayFrames()
{
    // 调用 GameView 的加载和展示函数
    gameView->loadFramesAndDisplay();
}

void GamePage::setBackgroundImage(const QString &path)
{
    qDebug() << "[GamePage] setBackgroundImage called:" << path;
    if (gameView) {
        gameView->setBackgroundImage(path);  // 将背景图传递给 GameView
    }
}

void GamePage::onCharacterSpeedChanged(int speed)
{
    qDebug() << "[GamePage] onCharacterSpeedChanged called:" << speed;
    if (gameView) {
        gameView->setCharacterSpeed(speed);  // 传递速度给游戏视图处理
    }
}

void GamePage::mousePressEvent(QMouseEvent *event)
{
    //只处理鼠标左键的按下事件
    if (event->button() == Qt::LeftButton) {
        dragging = true;
        dragPosition = event->globalPosition() - frameGeometry().topLeft();
        event->accept();
    }
}

void GamePage::mouseMoveEvent(QMouseEvent *event)
{
    // 检查是否是鼠标左键按下的同时在移动（即拖拽）
    if (dragging && (event->buttons() & Qt::LeftButton)) {
        move((event->globalPosition() - dragPosition).toPoint());
        event->accept();
    }
}

void GamePage::mouseReleaseEvent(QMouseEvent *event)
{
    // 当鼠标左键释放时，停止拖拽
    if (event->button() == Qt::LeftButton) {
        dragging = false;
        event->accept();
    }
}

void GamePage::contextMenuEvent(QContextMenuEvent *event)
{
    // 在鼠标光标的位置显示右键菜单
    contextMenu->exec(event->globalPos());
}

void GamePage::showYearlyReport()
{
    if (!frameManager) { // 确保 frameManager 已经被初始化
        qWarning() << "PhotoFrameManager is not initialized in GamePage!";
        return;
    }
    YearlyReportDialog reportDialog(*frameManager, this); // 传递 frameManager
    reportDialog.exec();
}

void GamePage::openAIChatDialog()
{
    if (!chatDialog) 
    { 
        chatDialog = new AIChatDialog(this);

        connect(chatDialog, &AIChatDialog::requestStarted, this, [=]() {
            setEmotion(Thinking);
        });
        
        connect(chatDialog, &AIChatDialog::responseReceived, this, [=]() {
            setEmotion(Happy); 
        });

        connect(chatDialog, &QObject::destroyed, this, [=]() {
            chatDialog = nullptr;
        });
    }

    chatDialog->show();
    chatDialog->activateWindow();
}


// --- 情绪和动画更新 ---

void GamePage::updateAnimation()
{
    QString gifPath;
    switch (currentEmotion) 
    {
        case Thinking:
            gifPath = ":/characters/character/thinking.gif"; 
            break;
        case Happy:
            gifPath = ":/characters/character/happy.gif";
            break;
        case Normal:
        default:
            gifPath = ":/characters/character/shime1.gif";
            break;
    }

    // 只有当动画文件改变时才重新设置，以避免不必要的重载
    if (movie->fileName() != gifPath) 
    {
        movie->setFileName(gifPath);
        movie->start();
    }
}

void GamePage::setEmotion(PetEmotion emotion)
{
    // 如果情绪没有变化，就什么也不做
    if (currentEmotion == emotion) 
    {
        return;
    }
    
    // 更新情绪状态，并调用函数更新动画
    currentEmotion = emotion;
    updateAnimation();
}

/*上面的代码中，
":/characters/character/thinking.gif" 
和 ":/characters/character/happy.gif" 是占位符。
需要准备两个这样的GIF动画，并将它们添加到你的Qt资源文件 (characters.qrc) 中，
路径要与代码中的一致。如果暂时没有，程序也能运行，只是在切换情绪时不会改变动画。*/


GamePage::~GamePage(){}
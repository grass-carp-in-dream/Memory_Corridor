#include "gamepage.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include "PhotoFrameManager.h"


GamePage::GamePage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

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

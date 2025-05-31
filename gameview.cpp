#include "gameview.h"
#include <QDebug>
#include <QKeyEvent>
#include <QResizeEvent>
#include "PhotoFrameManager.h"
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QVBoxLayout>
#include <QLabel>
#include "PhotoFrameItem.h"
#include <QApplication>

// 构造函数：初始化定时器，连接更新槽函数
GameView::GameView(QWidget *parent)
    : QGraphicsView(parent), scene(nullptr), backgroundItem(nullptr), characterItem(nullptr)
{
    moveTimer = new QTimer(this);   // 控制移动更新
    animTimer = new QTimer(this);   // 控制动画帧播放

    connect(moveTimer, &QTimer::timeout, this, &GameView::updateCharacterPosition);
    connect(animTimer, &QTimer::timeout, this, &GameView::updateAnimationFrame);

    moveTimer->start(30);   // 每 30ms 更新一次移动
    animTimer->start(100);  // 每 100ms 更新一次动画帧

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // 初始化默认值
    characterScale = 1.0;
    characterYOffset = 0;
    groundY = 600;   // 假定地面Y坐标600，可按需调整
    frameWidth = 128; // 假设每帧宽度64像素，按你的资源修改
    frameHeight = 0; // 初始化为0，后续加载时赋值
    currentFacing = FaceRight;
    currentFrame = 0;
    isJumping = false;
    verticalVelocity = 0;
    gravity = 1.0;
    jumpSpeed = 15;
    movingLeft = movingRight = false;
    isSpeedUp = false;
    normalSpeed = 5;
    acceleratedSpeed = 10;
}

// 初始化场景，加载背景和角色，设置初始状态
void GameView::initScene()
{
    scene = new QGraphicsScene(this);
    this->setScene(scene);

    scene->setSceneRect(0, 0, 50000, 600);

    // 背景图平铺
    QPixmap bgPixmap(":/new/prefix1/background5.png");
    int bgWidth = bgPixmap.width();

    int tileCount = static_cast<int>(scene->width() / bgWidth) + 2;
    int bgYOffset = -500; // 背景偏移量，根据需要调整

    for (int i = 0; i < tileCount; ++i) {
        QGraphicsPixmapItem *tile = scene->addPixmap(bgPixmap);
        tile->setZValue(-1);
        tile->setPos(i * bgWidth, bgYOffset);
    }

    // 载入角色精灵帧
    QPixmap spriteSheet(":/new/prefix1/Girl_1.png");
    frameHeight = spriteSheet.height();

    int frameCount = spriteSheet.width() / frameWidth;
    walkFrames.clear();

    for (int i = 0; i < frameCount; i++) {
        walkFrames.append(spriteSheet.copy(i * frameWidth, 0, frameWidth, frameHeight));
    }

    if (walkFrames.isEmpty()) {
        qDebug() << "角色帧图加载失败";
        return;
    }

    characterItem = new QGraphicsPixmapItem(walkFrames[0]);
    characterItem->setTransformationMode(Qt::SmoothTransformation);

    // 设置旋转中心为图形中心，方便翻转
    characterItem->setTransformOriginPoint(characterItem->boundingRect().center());

    scene->addItem(characterItem);

    // 用setTransform统一设置缩放，初始方向默认向右（scaleX正）
    characterItem->setTransform(QTransform().scale(characterScale, 1.0));

    // 设置角色初始位置，关键计算公式：
    // posY = groundY - frameHeight * characterScale + characterYOffset
    characterItem->setPos(0, groundY - frameHeight * characterScale + characterYOffset);

    centerOn(characterItem);
}


// 角色移动和跳跃逻辑
void GameView::updateCharacterPosition()
{
    if (!characterItem) return;
    int moveSpeed = isSpeedUp ? acceleratedSpeed : normalSpeed;

    QPointF pos = characterItem->pos();
    int w = frameWidth * characterScale;
    int h = frameHeight * characterScale;

    // 角色脚底应该站立的y坐标，包含偏移调整
    double groundLevel = groundY - h + characterYOffset;

    // 左右移动
    if (movingLeft && !movingRight) {
        pos.setX(pos.x() - moveSpeed);
        if (pos.x() < 0) pos.setX(0);

        if (currentFacing != FaceLeft) {
            currentFacing = FaceLeft;

            // 翻转并缩放，scaleX取负值实现水平翻转
            characterItem->setTransformOriginPoint(characterItem->boundingRect().center());
            characterItem->setTransform(QTransform().scale(-characterScale, characterScale));

            // 翻转时向右补偿宽度，防止跳动
            pos.setX(pos.x() + w);
        }
    }
    else if (movingRight && !movingLeft) {
        pos.setX(pos.x() + moveSpeed);
        if (pos.x() > scene->width() - w)
            pos.setX(scene->width() - w);

        if (currentFacing != FaceRight) {
            currentFacing = FaceRight;

            // 正常方向，scaleX为正值
            characterItem->setTransformOriginPoint(characterItem->boundingRect().center());
            characterItem->setTransform(QTransform().scale(characterScale, characterScale));


            // 翻转时向左补偿宽度，防止跳动
            pos.setX(pos.x() - w);
        }
    }

    // 跳跃和重力处理
    if (isJumping) {
        verticalVelocity -= gravity;
        pos.setY(pos.y() - verticalVelocity);

        // 到达或超过地面高度，停止跳跃，角色落地
        if (qAbs(pos.y() - groundLevel) < 1.0 || pos.y() > groundLevel) {
            pos.setY(groundLevel);
            isJumping = false;
            verticalVelocity = 0;
        }
    }

    characterItem->setPos(pos);

    // 视角跟随角色
    centerOn(characterItem);
}


// 循环播放动画帧
void GameView::updateAnimationFrame()
{
    if (walkFrames.isEmpty() || !characterItem) return;

    currentFrame = (currentFrame + 1) % walkFrames.size();
    characterItem->setPixmap(walkFrames[currentFrame]);
}

// 按键按下事件处理
void GameView::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        movingLeft = true;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        movingRight = true;
        break;
    case Qt::Key_W:
    case Qt::Key_Space:
        if (!isJumping) {
            isJumping = true;
            verticalVelocity = jumpSpeed;
        }
        break;
    case Qt::Key_Shift:
        isSpeedUp = true;
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

// 按键释放事件处理
void GameView::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_A:
    case Qt::Key_Left:
        movingLeft = false;
        break;
    case Qt::Key_D:
    case Qt::Key_Right:
        movingRight = false;
        break;
    case Qt::Key_Shift:
        isSpeedUp = false;
        break;
    default:
        QGraphicsView::keyReleaseEvent(event);
    }
}

// 通过图片路径设置角色动画精灵帧，重新加载动画帧（新增函数）
void GameView::setCharacterImage(const QString &imagePath) {
    qDebug() << "[GameView] setCharacterImage called with path:" << imagePath;
    QPixmap spriteSheet(imagePath);
    if (spriteSheet.isNull()) {
        qDebug() << "角色图片加载失败:" << imagePath;
        return;
    }

    frameHeight = spriteSheet.height();
    int frameCount = spriteSheet.width() / frameWidth;

    walkFrames.clear();
    for (int i = 0; i < frameCount; i++) {
        walkFrames.append(spriteSheet.copy(i * frameWidth, 0, frameWidth, frameHeight));
    }

    if (walkFrames.isEmpty()) {
        qDebug() << "角色帧图为空";
        return;
    }

    if (!characterItem) {
        characterItem = new QGraphicsPixmapItem(walkFrames[0]);
        characterItem->setTransformationMode(Qt::SmoothTransformation);
        characterItem->setTransformOriginPoint(characterItem->boundingRect().center());
        scene->addItem(characterItem);
    } else {
        characterItem->setPixmap(walkFrames[0]);
        characterItem->setTransformOriginPoint(characterItem->boundingRect().center());
    }

    if (currentFacing == FaceRight) {
        characterItem->setTransform(QTransform().scale(characterScale, characterScale));
    } else {
        characterItem->setTransform(QTransform().scale(-characterScale, characterScale));
    }

    characterItem->setPos(0, groundY - frameHeight * characterScale + characterYOffset);

    centerOn(characterItem);
    update();
}


// 设置角色缩放比例，同时自动调整位置（新增函数）
void GameView::setCharacterScale(double scale) {
    qDebug() << "[GameView] setCharacterScale called with scale:" << scale;
    characterScale = scale*3;

    if (characterItem) {
        if (currentFacing == FaceRight) {
            characterItem->setTransform(QTransform().scale(characterScale, characterScale));  // y轴也缩放
        } else {
            characterItem->setTransform(QTransform().scale(-characterScale, characterScale)); // y轴也缩放，x轴负号实现翻转
        }


        // 缩放后调整角色垂直位置，防止浮空或埋地
        characterItem->setPos(characterItem->pos().x(), groundY - frameHeight * characterScale + characterYOffset);
    }
    update();
}

// 设置角色垂直偏移量（站低点效果），并更新位置（新增函数）
void GameView::setCharacterYOffset(int yOffset) {
    qDebug() << "[GameView] setCharacterYOffset called with yOffset:" << yOffset;
    characterYOffset = yOffset*3;

    if (characterItem) {
        // 重新计算位置，保持脚底对齐地面+偏移
        characterItem->setPos(characterItem->pos().x(), groundY - frameHeight * characterScale + characterYOffset);
    }
    update();
}

// Qt绘制事件，保持默认行为
void GameView::paintEvent(QPaintEvent *event) {
    QGraphicsView::paintEvent(event);
}

QGraphicsScene* GameView::getScene() const {
    return scene;
}

void GameView::loadFramesAndDisplay()
{
    PhotoFrameManager frameManager;
    // 这里的文件路径要改成你们自己保存的json文件的绝对路径
    if (!frameManager.loadFromFile("D:/程设大作业/Memory_Corridor/json/data.json")) {
        qDebug() << "加载相框数据失败";
        return;
    }

    const QVector<PhotoFrameData> &frames = frameManager.getAllFrames();
    int xOffset = 100;
    int yOffset = 100;
    int frameWidth = 150;
    int frameHeight = 150;
    int margin = 100;

    for (const PhotoFrameData &frameData : frames) {
        QPixmap framePixmap(frameData.getImagePath());
        if (framePixmap.isNull()) {
            qDebug() << "相框图片加载失败：" << frameData.getImagePath();
            continue;
        }

        // 缩放图片以适应相框的大小
        framePixmap = framePixmap.scaled(frameWidth, frameHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        // 创建相框项并设置位置
        PhotoFrameItem *frameItem = new PhotoFrameItem(framePixmap, frameData.getImagePath());
        frameItem->setZValue(-1); // 确保相框在角色背景之下
        frameItem->setPos(xOffset, yOffset); // 设置相框位置

        // 添加描述文本
        QGraphicsTextItem *descriptionItem = scene->addText(frameData.getDescription());
        descriptionItem->setPos(xOffset, yOffset + frameHeight + 5);
        descriptionItem->setDefaultTextColor(Qt::white);
        descriptionItem->setTextWidth(frameWidth);

        // 添加时间戳
        QGraphicsTextItem *timestampItem = scene->addText(frameData.getCreationTime());
        timestampItem->setPos(xOffset, yOffset - 20);
        timestampItem->setDefaultTextColor(Qt::lightGray);

        // 更新水平位置
        xOffset += frameWidth + margin;
        scene->addItem(frameItem);  // 添加到场景中
    }

    update(); // 更新视图
}


void GameView::mouseDoubleClickEvent(QMouseEvent *event)
{
    // 获取双击位置
    QPointF clickPos = mapToScene(event->pos());

    // 查找该位置是否点击了相框
    QGraphicsItem *item = scene->itemAt(clickPos, QTransform());

    // 如果点击的项目是自定义的相框项
    if (item && item->type() == QGraphicsPixmapItem::Type) {
        PhotoFrameItem *frameItem = dynamic_cast<PhotoFrameItem*>(item);
        if (frameItem) {
            QString imagePath = frameItem->getImagePath();
            if (!imagePath.isEmpty()) {
                // 调用一个方法来展示放大图
                showZoomedImage(imagePath);
            }
        }
    }

    // 调用基类的双击事件处理
    QGraphicsView::mouseDoubleClickEvent(event);
}



void GameView::showZoomedImage(const QString &imagePath)
{
    // 创建一个新的窗口来展示图片
    QWidget *zoomedImageWindow = new QWidget(this);
    zoomedImageWindow->setWindowTitle("放大查看");

    QVBoxLayout *layout = new QVBoxLayout(zoomedImageWindow);

    QLabel *imageLabel = new QLabel(zoomedImageWindow);
    QPixmap zoomedPixmap(imagePath);

    // 放大图片
    imageLabel->setPixmap(zoomedPixmap.scaled(zoomedPixmap.size() * 0.5, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    layout->addWidget(imageLabel);

    zoomedImageWindow->resize(zoomedPixmap.size() * 0.5);

    // 获取屏幕信息
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        QRect screenGeometry = screen->availableGeometry();
        int centerX = screenGeometry.center().x() - zoomedImageWindow->width() / 2;
        int centerY = screenGeometry.center().y() - zoomedImageWindow->height() / 2;
        zoomedImageWindow->move(centerX, centerY); // 居中
    }

    // 安装事件过滤器
    zoomedImageWindow->installEventFilter(this);

    zoomedImageWindow->show();
}


bool GameView::eventFilter(QObject *watched, QEvent *event)
{
    // 检查事件是否来自我们创建的放大查看窗口
    if (watched->isWidgetType()) {
        QWidget *zoomedImageWindow = qobject_cast<QWidget*>(watched);
        if (zoomedImageWindow) {
            // 如果是鼠标点击事件并且是左键点击
            if (event->type() == QEvent::MouseButtonPress) {
                QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
                if (mouseEvent->button() == Qt::LeftButton) {
                    zoomedImageWindow->close();  // 点击鼠标左键关闭窗口
                    return true;  // 事件已处理，防止进一步传播
                }
            }
        }
    }

    // 调用基类的 eventFilter 方法处理其他事件
    return QGraphicsView::eventFilter(watched, event);
}

void GameView::setBackgroundImage(const QString &path) {
    if (!scene) return;

    scene->clear(); // 清空旧内容（包括背景、角色）
    qDebug() << "[GameView] setBackgroundImage with:" << path;
    QPixmap bgPixmap(path);
    if (bgPixmap.isNull()) {
        qDebug() << "背景图片加载失败:" << path;
        return;
    }

    int bgWidth = bgPixmap.width();
    int tileCount = static_cast<int>(scene->width() / bgWidth) + 2;
    int bgYOffset = -500;

    for (int i = 0; i < tileCount; ++i) {
        QGraphicsPixmapItem *tile = scene->addPixmap(bgPixmap);
        tile->setZValue(-1);
        tile->setPos(i * bgWidth, bgYOffset);
    }

    // 重新添加角色
    if (!walkFrames.isEmpty()) {
        characterItem = new QGraphicsPixmapItem(walkFrames[0]);
        characterItem->setTransformationMode(Qt::SmoothTransformation);
        characterItem->setTransformOriginPoint(characterItem->boundingRect().center());
        scene->addItem(characterItem);
        characterItem->setTransform(QTransform().scale(
            currentFacing == FaceRight ? characterScale : -characterScale,
            characterScale));
        characterItem->setPos(0, groundY - frameHeight * characterScale + characterYOffset);
        centerOn(characterItem);
    }
}

void GameView::setCharacterSpeed(int speed) {
    qDebug() << "[GameView] setCharacterSpeed called with speed:" << speed;
    normalSpeed = speed;
    acceleratedSpeed = speed * 2;  // 可根据需要自行定义加速模式的速度
}


#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QKeyEvent>
#include <QVector>

// 游戏视图类：用于显示横向卷轴地图与角色控制
class GameView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GameView(QWidget *parent = nullptr);

    // 初始化场景，添加背景图和角色等
    void initScene();

    // 设置背景图片路径
    void setBackgroundImage(const QString &path);

    // 设置角色图片路径
    void setCharacterImage(const QString &path);

    // 设置角色缩放比例
    void setCharacterScale(double scale);

    // 设置角色垂直偏移（角色上下浮动）
    void setCharacterYOffset(int offset);
    QGraphicsScene* getScene() const;
    void loadFramesAndDisplay();
    void showZoomedImage(const QString &imagePath);
    bool eventFilter(QObject *watched, QEvent *event) override;


protected:
    // 处理键盘按下事件（控制角色移动、跳跃）
    void keyPressEvent(QKeyEvent *event) override;

    // 处理键盘释放事件（停止移动）
    void keyReleaseEvent(QKeyEvent *event) override;

    // 重写绘制事件，实现自定义绘制
    void paintEvent(QPaintEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *event) override;

private slots:
    // 定时器槽函数：更新角色位置（移动+跳跃）
    void updateCharacterPosition();

    // 定时器槽函数：切换角色动画帧（走路动画）
    void updateAnimationFrame();


private:
    QGraphicsScene *scene;                   // 场景容器，放背景、角色等
    QGraphicsPixmapItem *backgroundItem;     // 背景图项
    QGraphicsPixmapItem *characterItem;      // 角色图项

    QVector<QPixmap> walkFrames;             // 存储走路动画帧图
    int currentFrame = 0;                    // 当前动画帧编号

    // 角色移动相关
    float moveSpeed = 5.0f;                  // 普通移动速度
    float runSpeed = 8.0f;                   // 加速速度（按下Shift时，可拓展）
    bool movingLeft = false;                 // 是否正在向左移动
    bool movingRight = false;                // 是否正在向右移动
    bool isJumping = false;                  // 是否正在跳跃

    // 跳跃相关参数
    float jumpSpeed = 15.0f;                 // 初始跳跃速度
    float verticalVelocity = 0.0f;           // 当前竖直速度
    float gravity = 0.8f;                    // 重力加速度
    int groundY = 200;                       // 地面Y坐标（角色落地位置）

    // 定时器
    QTimer *moveTimer;                       // 负责不断更新角色位置（30ms一次）
    QTimer *animTimer;                       // 负责不断切换动画帧（100ms一次）

    // 角色外观参数
    int frameWidth = 64;  // 角色单帧宽度
    int frameHeight = 0;  // 角色单帧高度，后面赋值
    enum FacingDirection { FaceRight, FaceLeft };
    FacingDirection currentFacing = FaceRight;

    bool isSpeedUp = false;  // 是否按下 Shift 加速
    int normalSpeed = 5;
    int acceleratedSpeed = 10;

    QPixmap characterPixmap;  // 角色图像
    double characterScale = 1.0f;  // 缩放比例，默认1倍
    int characterYOffset = 0;  // Y轴偏移量，默认0

};
#endif // GAMEVIEW_H


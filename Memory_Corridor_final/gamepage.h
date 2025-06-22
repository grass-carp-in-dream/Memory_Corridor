#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include "gameview.h"
#include "PhotoFrameManager.h"
#include <QPoint>
#include <QMouseEvent>
#include <QMenu>
#include <QContextMenuEvent>
#include <QLabel> 


//以下是桌宠“情绪部分”
class AIChatDialog;


class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);
    ~GamePage();

    // 定义宠物的情绪状态
    enum PetEmotion {
        Normal,   // 正常状态
        Thinking, // 思考中
        Happy     // 开心
    };

    void setEmotion(PetEmotion emotion);


    // 设置焦点到游戏视图
    void setFocusToGameView();

    // 设置角色图像
    void setCharacterImage(const QString &imagePath);

    // 设置角色的缩放比例
    void setCharacterScale(double scale);

    // 设置角色的垂直偏移量
    void setCharacterYOffset(int offset);

signals:
    // 返回主菜单
    void backToMainRequested();

    // 显示设置页面
    void showSettingsPage();
public slots:
    // 加载相框并显示
    void loadAndDisplayFrames();
    void setBackgroundImage(const QString &path);
    void onCharacterSpeedChanged(int speed);

protected:
    // 重写QWidget的鼠标事件函数
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void showYearlyReport();
    void openAIChatDialog();

private:
    QLabel *m_petLabel;
    QMovie *movie;

    QMenu *contextMenu;// 右键菜单对象

    //下面这两个成员变量用于拖拽
    QPointF dragPosition; // 用于记录鼠标和窗口的相对位置
    bool dragging;       // 拖拽状态的标志

    AIChatDialog *chatDialog; // AI 对话窗口的指针
    PetEmotion currentEmotion; // 保存当前的情绪状态

    //私有辅助函数，用于根据情绪更新动画
    void updateAnimation();

private:
    // 游戏视图对象
    GameView *gameView;

    // 按钮对象
    QPushButton *backButton;
    QPushButton *settingsButton;
    QPushButton *loadButton;

    // 相框管理器对象
    PhotoFrameManager *frameManager;

    // 角色的缩放比例和垂直偏移量
    double m_characterScale;
    int m_characterYOffset;
};

#endif // GAMEPAGE_H

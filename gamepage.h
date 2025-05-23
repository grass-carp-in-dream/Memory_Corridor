#ifndef GAMEPAGE_H
#define GAMEPAGE_H

#include <QWidget>
#include <QPushButton>
#include "gameview.h"
#include "PhotoFrameManager.h"

class GamePage : public QWidget
{
    Q_OBJECT

public:
    explicit GamePage(QWidget *parent = nullptr);

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

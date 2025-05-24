#ifndef GAMESETTINGSPAGE_H
#define GAMESETTINGSPAGE_H

#include <QWidget>
#include <QSlider>
#include <QVBoxLayout>
#include <QLabel>

class GameSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit GameSettingsPage(QWidget *parent = nullptr);

signals:
    void characterSpeedChanged(int speed);  // 发出角色速度变化信号

private slots:
    void onSpeedSliderChanged(int value);

private:
    QSlider *speedSlider;
    QLabel *speedLabel;
};

#endif // GAMESETTINGSPAGE_H

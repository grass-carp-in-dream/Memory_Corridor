#ifndef MUSICSETTINGSPAGE_H
#define MUSICSETTINGSPAGE_H

#include <QWidget>

class QComboBox;
class QSlider;
class QCheckBox;

class MusicSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit MusicSettingsPage(QWidget *parent = nullptr);

signals:
    void musicTrackChanged(const QString &filePath);  // 更换音乐
    void musicVolumeChanged(int volume);              // 音量调整
    void musicMutedChanged(bool muted);               // 静音切换

private:
    QComboBox *trackComboBox;
    QSlider *volumeSlider;
    QCheckBox *muteCheckBox;

    QString getTrackPathByName(const QString &name);
};

#endif // MUSICSETTINGSPAGE_H

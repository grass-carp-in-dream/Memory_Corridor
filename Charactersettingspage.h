#ifndef CHARACTERSETTINGSPAGE_H
#define CHARACTERSETTINGSPAGE_H

#include <QWidget>
#include <QSlider>
#include <QComboBox>
#include <QLabel>

class CharacterSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit CharacterSettingsPage(QWidget *parent = nullptr);

signals:
    void characterImageChanged(const QString &imagePath);
    void characterSizeChanged(double scale);
    void characterYOffsetChanged(int yOffset);
    void closed();

private slots:
    void onImageChanged(int index);
    void onSizeChanged(int value);
    void onYOffsetChanged(int value);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QComboBox *imageComboBox;
    QSlider *sizeSlider;
    QSlider *yOffsetSlider;
};

#endif // CHARACTERSETTINGSPAGE_H

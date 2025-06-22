#ifndef BACKGROUNDSETTINGSPAGE_H
#define BACKGROUNDSETTINGSPAGE_H

#include <QWidget>

class QLabel;
class QPushButton;
class QGridLayout;

class BackgroundSettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit BackgroundSettingsPage(QWidget *parent = nullptr);

signals:
    void backgroundImageChanged(const QString &path);

private slots:
    void onDefaultImageClicked();
    void onChooseImage();

private:
    QLabel *previewLabel;
    QPushButton *chooseButton;
    QVector<QPushButton*> defaultButtons;
    QStringList defaultImagePaths;
};

#endif // BACKGROUNDSETTINGSPAGE_H

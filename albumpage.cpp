#include "albumpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QResizeEvent>
#include<QInputDialog>
#include "dateselectdialog.h"
#include <QFileDialog>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include<QMessageBox>

AlbumPage::AlbumPage(QWidget *parent)
    : QWidget(parent)
{
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // 创建按钮
    createButtons();
    mainLayout->addWidget(buttonWidget);

    // 创建滚动内容
    QScrollArea *area = createFlowLayout();
    mainLayout->addWidget(area);

    setLayout(mainLayout);

    // 这里的文件路径要改成你们自己保存的json文件的绝对路径
    QString defaultPath = "C:\\Users\\27124\\Desktop";
    if (QFile::exists(defaultPath)) {
        loadAlbumFromFile(defaultPath);
    }

}


/**
 * @brief 创建按钮区，包括添加相框按钮
 */
void AlbumPage::createButtons()
{
    buttonWidget = new QWidget(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(10);

    addButton = new QPushButton(tr("添加相框"), buttonWidget);
    saveButton = new QPushButton(tr("保存相册"), buttonWidget);
    backButton = new QPushButton(tr("返回主页面"), buttonWidget);
    loadButton = new QPushButton("加载相册",buttonWidget);
    backToGameButton = new QPushButton(tr("返回游戏页面"), buttonWidget);

    // 点击“添加相框”按钮时，触发 onAddPhoto 槽函数
    connect(addButton, &QPushButton::clicked, this, &AlbumPage::onAddPhoto);
    connect(saveButton, &QPushButton::clicked, this, &AlbumPage::onSaveButtonClicked);
    connect(backButton, &QPushButton::clicked, this, &AlbumPage::onBackButtonClicked);
    connect(loadButton, &QPushButton::clicked, this, &AlbumPage::onLoadAlbum);
    connect(backToGameButton, &QPushButton::clicked, this, &AlbumPage::onBackToGameButtonClicked);

    buttonLayout->addStretch();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(backToGameButton);
    buttonLayout->addStretch();
}

void AlbumPage::onBackButtonClicked()
{
    emit backToMainRequested();
}

/**
 * @brief 初始化FlowLayout布局，用于流式排列相框
 */
QScrollArea* AlbumPage::createFlowLayout()
{
    // 创建滚动区域
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);  // 只垂直滚动
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // 创建容器，并设置 flowLayout 作为它的布局
    scrollContent = new QWidget(scrollArea);
    flowLayout = new FlowLayout(scrollContent, 10, 10, 10);
    scrollContent->setLayout(flowLayout);

    // 设置滚动区域的内容
    scrollArea->setWidget(scrollContent);

    return scrollArea;
}



/**
 * @brief 响应添加相框按钮点击，创建新的PhotoFrameWidget并添加到布局
 */
void AlbumPage::onAddPhoto()
{
    DateSelectDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    QDate selectedDate = dialog.selectedDate();

    PhotoFrameWidget *frame = new PhotoFrameWidget(this);
    frame->setDate(selectedDate);

    connect(frame, &PhotoFrameWidget::requestDelete, this, &AlbumPage::onRemovePhoto);

    photoFrames.append(frame);
    sortPhotoFrames();
    adjustPhotoFrameSizes();
}


/**
 * @brief 响应相框的删除请求，从布局和列表中移除该相框并销毁
 * @param frame 需要删除的相框指针
 */
void AlbumPage::onRemovePhoto(PhotoFrameWidget *frame)
{
    flowLayout->removeWidget(frame);
    photoFrames.removeOne(frame);
    frame->deleteLater();
    sortPhotoFrames(); // <<< 重新排序
    // 重新调整剩余相框大小
    adjustPhotoFrameSizes();
}

/**
 * @brief 窗口大小发生变化时调用，调整相框和按钮尺寸
 * @param event 窗口大小变化事件
 */
void AlbumPage::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    // 调整相框和按钮尺寸以适应新窗口大小
    adjustPhotoFrameSizes();
}

/**
 * @brief 根据当前窗口宽度，动态调整相框和按钮的尺寸
 */
void AlbumPage::adjustPhotoFrameSizes()
{
    if (photoFrames.isEmpty())
        return;

    // 计算可用宽度，减去FlowLayout的左右边距
    int availableWidth = this->width() - flowLayout->contentsMargins().left() - flowLayout->contentsMargins().right();

    // 相框尺寸限制，防止过大或过小
    int maxFrameWidth = 180;
    int minFrameWidth = 80;

    // 估算每行能放几个相框，包含间距10像素
    int perRow = qMax(1, availableWidth / (maxFrameWidth + 10));
    int frameWidth = availableWidth / perRow - 10;

    // 限定相框宽度范围
    frameWidth = qBound(minFrameWidth, frameWidth, maxFrameWidth);

    // 调整每个相框大小，增加高度以容纳描述区域
    for (auto frame : photoFrames) {
        frame->setFixedSize(frameWidth, frameWidth + 30);
    }

    // 按钮大小随窗口宽度调整，保证适当大小
    int btnWidth = qMax(80, availableWidth / 8);
    int btnHeight = 30;
    addButton->setFixedSize(btnWidth, btnHeight);
}
/**
 * @brief 排序函数
 */
void AlbumPage::sortPhotoFrames()
{
    // 使用 std::sort 根据日期 + 时间戳排序（日期早的排前面，日期相同则按添加顺序）
    std::sort(photoFrames.begin(), photoFrames.end(), [](PhotoFrameWidget *a, PhotoFrameWidget *b) {
        if (a->getDate() != b->getDate())
            return a->getDate() < b->getDate();
        else
            return a->getTimestamp() < b->getTimestamp();
    });

    // 清空布局并重新添加
    QLayoutItem *child;
    while ((child = flowLayout->takeAt(0)) != nullptr) {
        delete child;
    }

    for (PhotoFrameWidget *frame : photoFrames) {
        flowLayout->addWidget(frame);
    }
}

void AlbumPage::onSaveButtonClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
                                                    tr("保存相册文件"), "", tr("JSON 文件 (*.json);;所有文件 (*)"));

    if (filePath.isEmpty())
        return;

    saveAlbumToFile(filePath);

    QMessageBox::information(this, tr("保存成功"), tr("相册已保存到:\n%1").arg(filePath));
}

// 保存所有相框数据到指定文件路径
void AlbumPage::saveAlbumToFile(const QString &filePath)
{
    QJsonArray array;
    for (auto frame : photoFrames) {
        array.append(frame->toJson());
    }

    QJsonDocument doc(array);
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        // 这里可以弹个提示框，提示保存失败
        return;
    }
    file.write(doc.toJson());
    file.close();
}

// 从指定文件加载相框数据，清空旧相框，重建新相框
void AlbumPage::loadAlbumFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return; // 打开失败

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray())
        return; // 格式不对

    // 清空旧相框
    for (auto frame : photoFrames) {
        flowLayout->removeWidget(frame);
        frame->deleteLater();
    }
    photoFrames.clear();

    QJsonArray array = doc.array();
    for (auto value : array) {
        if (!value.isObject())
            continue;

        QJsonObject obj = value.toObject();

        PhotoFrameWidget *frame = new PhotoFrameWidget(this);
        frame->fromJson(obj);

        // 连接删除信号
        connect(frame, &PhotoFrameWidget::requestDelete, this, &AlbumPage::onRemovePhoto);

        photoFrames.append(frame);
        flowLayout->addWidget(frame);
    }

    adjustPhotoFrameSizes();
    sortPhotoFrames(); // 如果你之前有排序方法，调用一下保持顺序
}

void AlbumPage::onLoadAlbum()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("选择相册文件"),
                                                    QDir::homePath(),
                                                    tr("JSON 文件 (*.json)"));

    if (!filePath.isEmpty()) {
        loadAlbumFromFile(filePath);
    }
}

void AlbumPage::onBackToGameButtonClicked()
{
    emit backToGameRequested();  // 发射信号，通知返回游戏页面
}




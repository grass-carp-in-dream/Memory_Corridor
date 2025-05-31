#include "photoframewidget.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QLabel>
#include "ClickableLabel.h"
#include <QCalendarWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QJsonObject>
#include <QJsonValue>

// 构造函数：初始化控件，设置布局和信号槽
PhotoFrameWidget::PhotoFrameWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this); // 垂直布局，作为整个控件的主布局
    layout->setContentsMargins(5,5,5,5);         // 设置布局边距

    // 初始化图片显示标签，使用自定义的可点击Label
    imageLabel = new ClickableLabel(this);
    imageLabel->setFixedSize(120, 120);          // 固定大小
    imageLabel->setAlignment(Qt::AlignCenter);   // 内容居中
    imageLabel->setText("点击添加图片");         // 默认文本提示
    imageLabel->setStyleSheet("border: 1px solid gray;"); // 边框样式

    // 初始化描述输入框，设置提示文字
    descriptionEdit = new QLineEdit(this);
    descriptionEdit->setPlaceholderText("输入描述...");
    // 初始化日期输入框， 设置提示文字
    dateLabel = new QLabel("日期", this);

    // 将图片标签和描述输入框添加到布局中
    layout->addWidget(imageLabel);
    layout->addWidget(descriptionEdit);
    layout->addWidget(dateLabel);

    // 连接图片标签的点击信号到槽函数，带鼠标按键信息
    connect(imageLabel, &ClickableLabel::clicked, this, &PhotoFrameWidget::onImageClicked);

    creationTime = QDateTime::currentDateTime(); // 用于精细排序
}

// 响应图片标签点击事件，根据鼠标按键执行不同操作
void PhotoFrameWidget::onImageClicked(Qt::MouseButton button)
{
    if (button == Qt::LeftButton) {
        if (!imagePath.isEmpty()) {
            // 如果有原图路径，说明已加载过，点击则查看原图
            showImageViewer();
        } else {
            // 否则打开文件选择器选择图片
            QString fileName = QFileDialog::getOpenFileName(this, "选择图片", "", "图片文件 (*.png *.jpg *.jpeg *.bmp)");
            if (!fileName.isEmpty()) {
                QPixmap pixmap(fileName);
                if (!pixmap.isNull()) {
                    setImage(fileName);  // 使用路径版本，保存 imagePath
                }
            }
        }
    }
    else if (button == Qt::RightButton) {
            askDeleteConfirmation();
    }
}


// 创建并显示一个新窗口，放大查看当前图片
void PhotoFrameWidget::showImageViewer()
{
    if (imagePath.isEmpty()) return; // 没有原图路径就不操作

    QPixmap originalPixmap(imagePath);  // 用原图路径重新加载
    if (originalPixmap.isNull()) return;

    QLabel *viewer = new QLabel;
    viewer->setAttribute(Qt::WA_DeleteOnClose);
    viewer->setWindowTitle("查看图片");
    viewer->setPixmap(originalPixmap);  // 使用高清原图
    viewer->setScaledContents(false);   // 不拉伸，保持原尺寸
    viewer->adjustSize();               // 标签尺寸适应图片
    viewer->show();
}

// 弹出删除确认对话框，用户确认则发送删除请求信号
void PhotoFrameWidget::askDeleteConfirmation()
{
    auto ret = QMessageBox::question(this, "删除确认", "确定删除这个相框吗？", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        emit requestDelete(this);         // 发送信号，请求删除自己
    }
}

// 设置图片，自动按标签大小缩放，并清除默认文本
void PhotoFrameWidget::setImage(const QPixmap &pix)
{
    imageLabel->setPixmap(pix.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setText("");              // 清除文字提示
}
void PhotoFrameWidget::setImage(const QString &path)
{
    imagePath = path;  // 保存路径
    QPixmap pixmap(path);
    imageLabel->setPixmap(pixmap.scaled(imageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageLabel->setText("");
}


// 获取当前显示的图片，若无图片则返回空QPixmap
QPixmap PhotoFrameWidget::image() const
{
    if (!imageLabel->pixmap().isNull())
        return imageLabel->pixmap();     // 注意：pixmap() 返回指针，这里解引用返回对象
    return QPixmap();
}

// 设置描述文本
void PhotoFrameWidget::setDescription(const QString &text)
{
    descriptionEdit->setText(text);
}

// 获取描述文本
QString PhotoFrameWidget::description() const
{
    return descriptionEdit->text();
}

/**
 * @brief 设置该相框的拍摄日期，并更新显示标签
 * @param d 拍摄日期（QDate 格式）
 */

void PhotoFrameWidget::setDate(const QDate &d) {
    date = d;
    dateLabel->setText("拍摄日期: " + d.toString("yyyy-MM-dd"));
}

/**
 * @brief 获取该相框的拍摄日期
 * @return 相框记录的日期（QDate 格式）
 */
QDate PhotoFrameWidget::getDate() const {
    return date;
}

/**
 * @brief 获取该相框的创建时间戳，用于在相同日期下判断先后顺序
 * @return 相框创建时的时间戳（QDateTime 格式）
 */
QDateTime PhotoFrameWidget::getTimestamp() const {
    return creationTime;
}

QString PhotoFrameWidget::getDescription() const {
    return descriptionEdit->text();
}


void PhotoFrameWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->button() != Qt::LeftButton)
        return;

    QDialog dialog(this);
    dialog.setWindowTitle("编辑相框");

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    // 日期编辑器
    QLabel *dateLabel = new QLabel("拍摄日期:");
    QCalendarWidget *calendar = new QCalendarWidget();
    calendar->setSelectedDate(date);

    // 描述编辑器
    QLabel *descLabel = new QLabel("图片描述:");
    QLineEdit *descEdit = new QLineEdit(this);
    descEdit->setText(description());

    // 新增：更换图片按钮
    QPushButton *changeImageButton = new QPushButton("更换图片");

    // 记录临时选中的图片路径
    QString newImagePath = imagePath; // imagePath是成员变量，存当前图片路径

    connect(changeImageButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(this, "选择图片", QString(),
                                                        "图片文件 (*.png *.jpg *.jpeg *.bmp *.gif)");
        if (!fileName.isEmpty()) {
            newImagePath = fileName; // 更新临时路径
        }
    });

    QPushButton *okButton = new QPushButton("确定");
    QPushButton *cancelButton = new QPushButton("取消");

    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(okButton);
    btnLayout->addWidget(cancelButton);

    layout->addWidget(dateLabel);
    layout->addWidget(calendar);
    layout->addWidget(descLabel);
    layout->addWidget(descEdit);
    layout->addWidget(changeImageButton); // 加按钮
    layout->addLayout(btnLayout);

    connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        setDate(calendar->selectedDate());
        setDescription(descEdit->text());

        // 如果路径改变了，更新图片
        if (newImagePath != imagePath) {
            setImage(newImagePath);
            imagePath = newImagePath; // 更新成员变量
        }

        emit requestUpdate();
    }
}

// 导出当前相框数据到JSON对象
QJsonObject PhotoFrameWidget::toJson() const
{
    QJsonObject obj;
    obj["imagePath"] = imagePath;                          // 图片路径
    obj["description"] = description();                    // 描述文本
    obj["date"] = date.toString(Qt::ISODate);             // 拍摄日期，ISO格式字符串
    obj["creationTime"] = creationTime.toString(Qt::ISODate); // 创建时间，ISO格式字符串
    return obj;
}

// 从JSON对象恢复相框数据
void PhotoFrameWidget::fromJson(const QJsonObject &obj)
{
    if (obj.contains("imagePath") && obj["imagePath"].isString()) {
        setImage(obj["imagePath"].toString());
    }

    if (obj.contains("description") && obj["description"].isString()) {
        setDescription(obj["description"].toString());
    }

    if (obj.contains("date") && obj["date"].isString()) {
        QDate d = QDate::fromString(obj["date"].toString(), Qt::ISODate);
        if (d.isValid())
            setDate(d);
    }

    if (obj.contains("creationTime") && obj["creationTime"].isString()) {
        QDateTime t = QDateTime::fromString(obj["creationTime"].toString(), Qt::ISODate);
        if (t.isValid())
            creationTime = t;
    }
}


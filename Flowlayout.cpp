#include "FlowLayout.h"
#include <QtWidgets>

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}


FlowLayout::~FlowLayout()
{
    // 析构时清理所有布局项，防止内存泄漏
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem *item)
{
    // 添加新的布局项
    itemList.append(item);
}

int FlowLayout::horizontalSpacing() const
{
    // 如果自定义了水平间距就返回，没有则智能计算
    if (m_hSpace >= 0)
        return m_hSpace;
    else
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int FlowLayout::verticalSpacing() const
{
    // 垂直间距逻辑同上
    if (m_vSpace >= 0)
        return m_vSpace;
    else
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
}

int FlowLayout::count() const
{
    // 返回子项数量
    return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    // 返回指定索引的布局项
    return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    // 取出并移除指定索引的布局项
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return Qt::Orientations();
}

bool FlowLayout::hasHeightForWidth() const
{
    // 支持高度根据宽度自动调整
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    // 计算给定宽度情况下，布局需要的高度
    return doLayout(QRect(0, 0, width, 0), true);
}

QSize FlowLayout::minimumSize() const
{
    // 计算最小尺寸，保证所有子项能显示
    QSize size;
    for (QLayoutItem *item : itemList)
        size = size.expandedTo(item->minimumSize());

    QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

void FlowLayout::setGeometry(const QRect &rect)
{
    // 设置布局区域，执行布局计算和控件位置设置
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    int x = rect.x();
    int y = rect.y();
    int lineHeight = 0;

    int spaceX = horizontalSpacing();
    if (spaceX == -1)
        spaceX = 10; // 默认水平间距
    int spaceY = verticalSpacing();
    if (spaceY == -1)
        spaceY = 10; // 默认垂直间距

    // 遍历所有布局项，逐行放置
    for (QLayoutItem *item : itemList) {
        int nextX = x + item->sizeHint().width() + spaceX;

        // 判断当前控件是否超出布局右边界，需要换行
        if (nextX - spaceX > rect.right() && lineHeight > 0) {
            x = rect.x(); // 重置x到左边界
            y = y + lineHeight + spaceY; // 换行，y增加上一行高度和垂直间距
            nextX = x + item->sizeHint().width() + spaceX;
            lineHeight = 0;
        }

        if (!testOnly)
            // 不是测试模式时，实际设置控件位置和大小
            item->setGeometry(QRect(QPoint(x, y), item->sizeHint()));

        x = nextX;
        // 更新当前行最高控件高度，用于换行时计算y偏移
        lineHeight = qMax(lineHeight, item->sizeHint().height());
    }
    // 返回整个布局需要的总高度
    return y + lineHeight - rect.y();
}

int FlowLayout::smartSpacing(QStyle::PixelMetric /*pm*/) const
{
    QObject *parent = this->parent();
    if (!parent) {
        return -1;
    } else if (parent->isWidgetType()) {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(QStyle::PM_LayoutHorizontalSpacing, nullptr, pw);
    } else {
        return static_cast<QLayout *>(parent)->spacing();
    }
}

#ifndef FLOWLAYOUT_H
#define FLOWLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QStyle>

/**
 * @brief 自定义的流式布局类，实现控件的自动换行排列。
 *
 * 作用类似网页里的“float”布局，子控件在一行放不下时自动换行到下一行。
 * 支持设置水平间距、垂直间距和边距。
 */
class FlowLayout : public QLayout
{
public:
    /**
     * @brief 构造函数，指定父控件和间距
     * @param parent 父控件指针
     * @param margin 外边距，默认-1表示使用系统默认
     * @param hSpacing 水平间距，默认-1表示使用系统默认
     * @param vSpacing 垂直间距，默认-1表示使用系统默认
     */
    explicit FlowLayout(QWidget *parent = nullptr, int margin = -1, int hSpacing = -1, int vSpacing = -1);


    /**
     * @brief 析构函数，清理所有子布局项
     */
    ~FlowLayout();

    /**
     * @brief 添加布局项，重写自 QLayout
     * @param item 要添加的布局项
     */
    void addItem(QLayoutItem *item) override;

    /**
     * @brief 获取水平间距
     * @return 水平间距像素值
     */
    int horizontalSpacing() const;

    /**
     * @brief 获取垂直间距
     * @return 垂直间距像素值
     */
    int verticalSpacing() const;

    /**
     * @brief 布局扩展方向，此布局不支持扩展，返回0
     */
    Qt::Orientations expandingDirections() const override;

    /**
     * @brief 是否支持根据宽度动态计算高度，返回true，支持动态高度
     */
    bool hasHeightForWidth() const override;

    /**
     * @brief 根据给定宽度计算布局所需高度
     * @param width 宽度
     * @return 计算出的高度
     */
    int heightForWidth(int width) const override;

    /**
     * @brief 返回布局中子项个数
     * @return 子项数量
     */
    int count() const override;

    /**
     * @brief 返回索引位置的布局项
     * @param index 索引
     * @return 布局项指针
     */
    QLayoutItem *itemAt(int index) const override;

    /**
     * @brief 返回布局最小尺寸，保证所有子项显示
     * @return 布局最小尺寸
     */
    QSize minimumSize() const override;
    QSize sizeHint() const override;


    /**
     * @brief 设置布局矩形区域，负责对子项布局和排布
     * @param rect 布局区域矩形
     */
    void setGeometry(const QRect &rect) override;

    /**
     * @brief 取出指定索引的布局项，布局不再管理它
     * @param index 索引
     * @return 被取出的布局项指针
     */
    QLayoutItem *takeAt(int index) override;

private:
    /**
     * @brief 具体执行布局的函数，负责计算和设置子项位置
     * @param rect 布局矩形区域
     * @param testOnly 如果为true，只计算高度不设置位置
     * @return 计算出的布局所需高度
     */
    int doLayout(const QRect &rect, bool testOnly) const;

    /**
     * @brief 获取智能间距，即根据系统风格或父控件自动返回合适的间距
     * @param pm QStyle中的像素度量类型
     * @return 间距像素值
     */
    int smartSpacing(QStyle::PixelMetric pm) const;

    QList<QLayoutItem *> itemList; ///< 子布局项列表
    int m_hSpace;                  ///< 水平间距
    int m_vSpace;                  ///< 垂直间距
};

#endif // FLOWLAYOUT_H

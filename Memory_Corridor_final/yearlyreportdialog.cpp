#include "YearlyReportDialog.h"
#include <QComboBox>
#include <QLabel>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QDebug>
#include <QPieSeries>
#include <QPieSlice>
#include <QChart>
#include <QChartView>

YearlyReportDialog::YearlyReportDialog(const PhotoFrameManager &manager, QWidget *parent)
    : QDialog(parent), m_manager(manager)
{
    setWindowTitle(tr("年度相框报告"));
    resize(800, 600); // 扩大窗口以适应图表

    createUI();
    populateYears();
}

void YearlyReportDialog::createUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 年份选择
    QHBoxLayout *yearLayout = new QHBoxLayout();
    yearLayout->addWidget(new QLabel(tr("选择年份:")));

    m_yearCombo = new QComboBox();
    yearLayout->addWidget(m_yearCombo);
    yearLayout->addStretch();

    mainLayout->addLayout(yearLayout);

    // 使用 TabWidget 分页显示报告和图表
    m_tabWidget = new QTabWidget();

    // 第一页：文字报告
    m_reportText = new QTextEdit();
    m_reportText->setReadOnly(true);
    m_tabWidget->addTab(m_reportText, tr("报告摘要"));

    // 第二页：月度分布图表
    m_monthlyChartView = new QChartView();
    m_monthlyChartView->setRenderHint(QPainter::Antialiasing);
    m_tabWidget->addTab(m_monthlyChartView, tr("月度分布"));

    // 第三页：关键词图表
    m_keywordChartView = new QChartView();
    m_keywordChartView->setRenderHint(QPainter::Antialiasing);
    m_tabWidget->addTab(m_keywordChartView, tr("关键词统计"));

    mainLayout->addWidget(m_tabWidget);

    // 连接信号
    connect(m_yearCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &YearlyReportDialog::updateReport);

    // 初始显示当前年份
    int currentYear = QDate::currentDate().year();
    m_yearCombo->setCurrentText(QString::number(currentYear));
}

void YearlyReportDialog::populateYears()
{
    int currentYear = QDate::currentDate().year();
    for (int year = currentYear; year >= currentYear - 10; --year) {
        m_yearCombo->addItem(QString::number(year));
    }
}

void YearlyReportDialog::updateReport(int index)
{
    if (index < 0) return;

    int year = m_yearCombo->itemText(index).toInt();
    auto summary = m_manager.getYearlySummary(year);

    m_reportText->setHtml(generateReportText(summary));
    updateCharts(summary);
}

QString YearlyReportDialog::generateReportText(const PhotoFrameManager::YearlySummary &summary) const
{
    QString report;
    report += QString("<h1>%1 年度相框报告</h1>").arg(m_yearCombo->currentText());
    report += QString("<h2>共 %1 个相框</h2>").arg(summary.totalFrames);

    // 月度分布
    report += "<h3>月度分布</h3><ul>";
    QStringList monthNames = {"一月", "二月", "三月", "四月", "五月", "六月",
                              "七月", "八月", "九月", "十月", "十一月", "十二月"};

    for (int i = 0; i < 12; ++i) {
        if (summary.framesPerMonth[i] > 0) {
            report += QString("<li>%1: %2 个相框</li>")
                          .arg(monthNames[i])
                          .arg(summary.framesPerMonth[i]);
        }
    }
    report += "</ul>";

    // 关键词分析
    if (!summary.keywordCounts.isEmpty()) {
        report += "<h3>热门关键词</h3><ul>";
        for (auto it = summary.keywordCounts.begin(); it != summary.keywordCounts.end(); ++it) {
            report += QString("<li>%1 (出现 %2 次)</li>")
                          .arg(it.key())
                          .arg(it.value());
        }
        report += "</ul>";
    }

    // 添加样式
    report = QString("<div style='font-family: Arial; padding: 20px;'>%1</div>").arg(report);
    return report;
}

void YearlyReportDialog::updateCharts(const PhotoFrameManager::YearlySummary &summary)
{
    qDebug() << "Keyword counts:" << summary.keywordCounts;  // 添加这行调试输出
    // 1. 月度分布柱状图
    QBarSeries *monthlySeries = new QBarSeries();
    QBarSet *monthlySet = new QBarSet("每月相框数量");

    QStringList months;
    for (int i = 0; i < 12; ++i) {
        *monthlySet << summary.framesPerMonth[i];
        months << QString("%1月").arg(i + 1);
    }
    monthlySeries->append(monthlySet);

    QChart *monthlyChart = new QChart();
    monthlyChart->addSeries(monthlySeries);
    monthlyChart->setTitle("月度相框数量分布");
    monthlyChart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(months);
    monthlyChart->addAxis(axisX, Qt::AlignBottom);
    monthlySeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    monthlyChart->addAxis(axisY, Qt::AlignLeft);
    axisY->setLabelFormat("%d");
    monthlySeries->attachAxis(axisY);

    m_monthlyChartView->setChart(monthlyChart);



    // 2. 关键词饼图（使用测试数据+调试）
    auto keywordCounts = summary.keywordCounts;  // 先用测试数据

    QPieSeries *pieSeries = new QPieSeries();
    for (auto it = keywordCounts.begin(); it != keywordCounts.end(); ++it) {
        if (it.value() > 0) {
            QPieSlice *slice = pieSeries->append(it.key(), it.value());
            slice->setLabelVisible(true);
            slice->setLabel(QString("%1 (%2次)").arg(it.key()).arg(it.value()));
        }
    }

    // 调试输出
    qDebug() << "PieSeries slices:" << pieSeries->count();
    if (pieSeries->count() == 0) {
        qDebug() << "错误：没有有效数据！";
        return;
    }

    QChart *chart = new QChart();
    chart->addSeries(pieSeries);
    chart->setTitle("关键词分布");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // 确保图表视图可见
    m_keywordChartView->setChart(chart);
    m_keywordChartView->setRenderHint(QPainter::Antialiasing);
    m_keywordChartView->setMinimumSize(400, 300);  // 临时设置尺寸
    m_keywordChartView->show();                    // 强制显示
}

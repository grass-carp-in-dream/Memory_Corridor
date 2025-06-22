#ifndef YEARLYREPORTDIALOG_H
#define YEARLYREPORTDIALOG_H

#include <QDialog>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QHorizontalBarSeries>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include "PhotoFrameManager.h"

QT_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_END_NAMESPACE// 使用 Qt Charts 命名空间

    class QLabel;
class QComboBox;
class QTextEdit;
class QVBoxLayout;
class QTabWidget;

class YearlyReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit YearlyReportDialog(const PhotoFrameManager &manager, QWidget *parent = nullptr);

private slots:
    void updateReport(int year);

private:
    void createUI();
    void populateYears();
    void updateCharts(const PhotoFrameManager::YearlySummary &summary);
    QString generateReportText(const PhotoFrameManager::YearlySummary &summary) const;

    const PhotoFrameManager &m_manager;
    QComboBox *m_yearCombo;
    QTextEdit *m_reportText;
    QTabWidget *m_tabWidget;
    QChartView *m_monthlyChartView;
    QChartView *m_keywordChartView;
};

#endif // YEARLYREPORTDIALOG_H

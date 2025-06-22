/********************************************************************************
** Form generated from reading UI file 'yearlyreportdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_YEARLYREPORTDIALOG_H
#define UI_YEARLYREPORTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_YearlyReportDialog
{
public:
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *summaryLabel;
    QGroupBox *monthlyGroupBox;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *monthlyChartLayout;
    QGroupBox *keywordGroupBox;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *keywordChartLayout;

    void setupUi(QDialog *YearlyReportDialog)
    {
        if (YearlyReportDialog->objectName().isEmpty())
            YearlyReportDialog->setObjectName("YearlyReportDialog");
        YearlyReportDialog->resize(1000, 700);
        YearlyReportDialog->setMinimumSize(QSize(1000, 700));
        verticalLayoutWidget = new QWidget(YearlyReportDialog);
        verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        verticalLayoutWidget->setGeometry(QRect(9, 9, 981, 681));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        summaryLabel = new QLabel(verticalLayoutWidget);
        summaryLabel->setObjectName("summaryLabel");
        QFont font;
        font.setPointSize(14);
        summaryLabel->setFont(font);
        summaryLabel->setAlignment(Qt::AlignCenter);
        summaryLabel->setWordWrap(true);

        verticalLayout->addWidget(summaryLabel);

        monthlyGroupBox = new QGroupBox(verticalLayoutWidget);
        monthlyGroupBox->setObjectName("monthlyGroupBox");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(monthlyGroupBox->sizePolicy().hasHeightForWidth());
        monthlyGroupBox->setSizePolicy(sizePolicy);
        verticalLayoutWidget_3 = new QWidget(monthlyGroupBox);
        verticalLayoutWidget_3->setObjectName("verticalLayoutWidget_3");
        verticalLayoutWidget_3->setGeometry(QRect(9, 19, 961, 261));
        monthlyChartLayout = new QVBoxLayout(verticalLayoutWidget_3);
        monthlyChartLayout->setObjectName("monthlyChartLayout");
        monthlyChartLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(monthlyGroupBox);

        keywordGroupBox = new QGroupBox(verticalLayoutWidget);
        keywordGroupBox->setObjectName("keywordGroupBox");
        sizePolicy.setHeightForWidth(keywordGroupBox->sizePolicy().hasHeightForWidth());
        keywordGroupBox->setSizePolicy(sizePolicy);
        verticalLayoutWidget_2 = new QWidget(keywordGroupBox);
        verticalLayoutWidget_2->setObjectName("verticalLayoutWidget_2");
        verticalLayoutWidget_2->setGeometry(QRect(9, 19, 961, 261));
        keywordChartLayout = new QVBoxLayout(verticalLayoutWidget_2);
        keywordChartLayout->setObjectName("keywordChartLayout");
        keywordChartLayout->setContentsMargins(0, 0, 0, 0);

        verticalLayout->addWidget(keywordGroupBox);

        verticalLayout->setStretch(0, 1);
        verticalLayout->setStretch(1, 3);
        verticalLayout->setStretch(2, 3);

        retranslateUi(YearlyReportDialog);

        QMetaObject::connectSlotsByName(YearlyReportDialog);
    } // setupUi

    void retranslateUi(QDialog *YearlyReportDialog)
    {
        YearlyReportDialog->setWindowTitle(QCoreApplication::translate("YearlyReportDialog", "\345\271\264\345\272\246\346\212\245\345\221\212", nullptr));
        summaryLabel->setText(QString());
        monthlyGroupBox->setTitle(QCoreApplication::translate("YearlyReportDialog", "\346\257\217\346\234\210\347\233\270\346\241\206\345\210\206\345\270\203", nullptr));
        keywordGroupBox->setTitle(QCoreApplication::translate("YearlyReportDialog", "\347\203\255\351\227\250\345\205\263\351\224\256\350\257\215", nullptr));
    } // retranslateUi

};

namespace Ui {
    class YearlyReportDialog: public Ui_YearlyReportDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_YEARLYREPORTDIALOG_H

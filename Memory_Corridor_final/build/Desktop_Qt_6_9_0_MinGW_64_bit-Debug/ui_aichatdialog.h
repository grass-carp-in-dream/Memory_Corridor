/********************************************************************************
** Form generated from reading UI file 'aichatdialog.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_AICHATDIALOG_H
#define UI_AICHATDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AIChatDialog
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QPushButton *sendButton;
    QTextEdit *inputEdit;
    QTextEdit *chatHistory;

    void setupUi(QDialog *AIChatDialog)
    {
        if (AIChatDialog->objectName().isEmpty())
            AIChatDialog->setObjectName("AIChatDialog");
        AIChatDialog->resize(654, 460);
        widget = new QWidget(AIChatDialog);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(20, 0, 259, 429));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        sendButton = new QPushButton(widget);
        sendButton->setObjectName("sendButton");

        gridLayout->addWidget(sendButton, 3, 0, 1, 1);

        inputEdit = new QTextEdit(widget);
        inputEdit->setObjectName("inputEdit");

        gridLayout->addWidget(inputEdit, 1, 0, 1, 1);

        chatHistory = new QTextEdit(widget);
        chatHistory->setObjectName("chatHistory");

        gridLayout->addWidget(chatHistory, 0, 0, 1, 1);


        retranslateUi(AIChatDialog);

        QMetaObject::connectSlotsByName(AIChatDialog);
    } // setupUi

    void retranslateUi(QDialog *AIChatDialog)
    {
        AIChatDialog->setWindowTitle(QCoreApplication::translate("AIChatDialog", "Dialog", nullptr));
        sendButton->setText(QCoreApplication::translate("AIChatDialog", "\345\217\221\351\200\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AIChatDialog: public Ui_AIChatDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_AICHATDIALOG_H

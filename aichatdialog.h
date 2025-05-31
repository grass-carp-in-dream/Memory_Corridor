#ifndef AICHATDIALOG_H
#define AICHATDIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace Ui {
class AIChatDialog;
}

class AIChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AIChatDialog(QWidget *parent = nullptr);
    ~AIChatDialog();

    signals:
    // 定义两个信号，用于通知 GamePage 我们的状态
    void requestStarted();   // 在发送网络请求前发射
    void responseReceived(); // 在收到网络回复后发射

private:
    Ui::AIChatDialog *ui;
    QNetworkAccessManager *networkManager; // 用于管理所有网络请求
    QString apiKey;                        // 用于存储你的 API Key

private slots:
    // 点击“发送”按钮时会自动调用的槽函数
    void on_sendButton_clicked();
    
    // 网络请求完成后会调用的槽函数
    void onNetworkReply(QNetworkReply *reply);

};

#endif // AICHATDIALOG_H
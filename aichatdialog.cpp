#include "aichatdialog.h"
#include "ui_aichatdialog.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

AIChatDialog::AIChatDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AIChatDialog)
{
    ui->setupUi(this);
    setWindowTitle("AI 对话");

    // 1. 初始化网络访问管理器
    networkManager = new QNetworkAccessManager(this);

    // 2. 连接网络管理器的 finished 信号到我们的处理槽函数
    //    这样，每当一个网络请求完成时，onNetworkReply 就会被调用
    connect(networkManager, &QNetworkAccessManager::finished, this, &AIChatDialog::onNetworkReply);

    // 3. 设置你的 API Key
    // !!! 重要: 请务必将 "sk-..." 替换为你自己的 DeepSeek API Key !!!
    // 为了安全，未来可以考虑从配置文件读取
    apiKey = "sk-6b183e64caf745288f1010e4850df8ff"; 

}

AIChatDialog::~AIChatDialog()
{
    delete ui;
}

void AIChatDialog::on_sendButton_clicked()
{
    // 1. 获取用户在 inputEdit 中输入的文本
    QString userInput = ui->inputEdit->toPlainText().trimmed(); // .trimmed() 可以去除首尾空格

    // 2. 如果用户什么都没输入，就直接返回
    if (userInput.isEmpty()) {
        return;
    }

    // 3. 将用户的输入添加到聊天记录 chatHistory 中，并用加粗显示 "You:"
    ui->chatHistory->append("<b>You:</b> " + userInput);

    // 4. 清空输入框 inputEdit
    ui->inputEdit->clear();
    
    // 5. 发射 requestStarted 信号，通知桌宠它该进入“思考中”的状态了
    emit requestStarted(); 
    
    // --- 构造API请求 ---
    // 6. 创建一个 QNetworkRequest 对象，并设置请求的URL
    QNetworkRequest request(QUrl("https://api.deepseek.com/chat/completions"));

    // 7. 设置请求头
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", ("Bearer " + apiKey).toUtf8()); // 注意 "Bearer " 后面有个空格

    // 8. 构造要发送的JSON数据体 (Payload)
    //    根据DeepSeek API文档，我们需要发送这样的结构：
    //    {
    //        "model": "deepseek-chat",
    //        "messages": [
    //            {"role": "user", "content": "用户输入的内容"}
    //        ]
    //    }
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["content"] = userInput;

    QJsonArray messagesArray;
    messagesArray.append(messageObject);

    QJsonObject payload;
    payload["model"] = "deepseek-chat"; // 选择模型，可以更改
    payload["messages"] = messagesArray;

    // 9. 将构造好的 QJsonObject 转换为 JSON 字符串，并发送 POST 请求
    networkManager->post(request, QJsonDocument(payload).toJson());
}

void AIChatDialog::onNetworkReply(QNetworkReply *reply)
{
    // 1. 发射 responseReceived 信号，通知桌宠思考结束
    emit responseReceived();

    // 2. 检查网络请求过程中是否有错误
    if (reply->error() != QNetworkReply::NoError) {
        // 如果有错误，在聊天记录中显示错误信息
        ui->chatHistory->append("<b>AI (Network Error):</b> " + reply->errorString());
        reply->deleteLater(); // 释放 QNetworkReply 对象
        return;
    }

    // 3. 读取API返回的所有数据
    QByteArray responseData = reply->readAll();
    
    // 4. 将返回的字节数据解析为 JSON 文档
    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
    QJsonObject jsonObj = jsonDoc.object();

    // 5. 解析JSON，提取AI的回复
    //    根据DeepSeek API文档，成功的回复结构类似：
    //    {
    //        "choices": [
    //            {
    //                "message": {
    //                    "role": "assistant",
    //                    "content": "AI回复的内容"
    //                }
    //            }
    //        ]
    //    }
    if (jsonObj.contains("choices")) {
        QJsonArray choices = jsonObj["choices"].toArray();
        if (!choices.isEmpty()) {
            QJsonObject firstChoice = choices.first().toObject();
            if (firstChoice.contains("message")) {
                QJsonObject messageObj = firstChoice["message"].toObject();
                if (messageObj.contains("content")) {
                    QString aiResponse = messageObj["content"].toString();
                    ui->chatHistory->append("<b>AI:</b> " + aiResponse);
                } else {
                    ui->chatHistory->append("<b>AI (Parse Error):</b> Response missing 'content' field.");
                }
            } else {
                 ui->chatHistory->append("<b>AI (Parse Error):</b> Response missing 'message' field in choice.");
            }
        } else {
            ui->chatHistory->append("<b>AI (API Error):</b> No choices returned.");
        }
    } else if (jsonObj.contains("error")) { // 检查API是否返回了错误信息
        QString errorMessage = jsonObj["error"].toObject()["message"].toString();
        ui->chatHistory->append("<b>AI (API Error):</b> " + errorMessage);
    } else {
        ui->chatHistory->append("<b>AI (Parse Error):</b> Unexpected JSON structure. Raw: " + QString(responseData));
    }
    
    // 6. 释放 QNetworkReply 对象，防止内存泄漏
    reply->deleteLater();
}

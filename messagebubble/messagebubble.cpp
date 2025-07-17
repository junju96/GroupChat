#include "messagebubble.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

MessageBubble::MessageBubble(const QString& sender, const QString& message,
                             const QDateTime& time, BubbleType type, QWidget* parent)
    : QFrame(parent)
{
    setObjectName(type == Self ? "selfBubble" : "otherBubble");

    // 设置气泡样式
    QString style = R"(
            #selfBubble {
                background-color: #95EC69;
                border-radius: 10px;
                margin: 5px;
                padding: 8px;
                min-height: 40px; /* 确保最小高度 */
            }
            #otherBubble {
                background-color: #FFFFFF;
                border-radius: 10px;
                margin: 5px;
                padding: 8px;
                min-height: 40px; /* 确保最小高度 */
            }
            #selfBubble QLabel, #otherBubble QLabel {
                font-size: 14px; /* 增大字体大小 */
            }
        )";
    setStyleSheet(style);

    QVBoxLayout* bubbleLayout = new QVBoxLayout(this);
    bubbleLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* headerLayout = new QHBoxLayout;
    QLabel* senderLabel = new QLabel(sender, this);
    senderLabel->setStyleSheet("font-weight: bold; color: #555555; font-size: 14px;background-color: transparent;");

    QLabel* timeLabel = new QLabel(time.toString("hh:mm"), this);
    timeLabel->setStyleSheet("color: #AAAAAA; font-size: 12px;background-color: transparent;");

    headerLayout->addWidget(senderLabel);
    headerLayout->addStretch();
    headerLayout->addWidget(timeLabel);

    QLabel* messageLabel = new QLabel(message, this);
    messageLabel->setWordWrap(true);
    messageLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    messageLabel->setStyleSheet("font-size: 14px; background-color: transparent;"); // 添加透明背景

    bubbleLayout->addLayout(headerLayout);
    bubbleLayout->addWidget(messageLabel);
}

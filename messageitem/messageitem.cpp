#include "messageitem.h"
#include <QHBoxLayout>
#include "avatarwidget/avatarwidget.h"

MessageItem::MessageItem(const QString& sender, const QString& message,
                         const QDateTime& time, MessageBubble::BubbleType type,
                         QWidget* parent)
    : QWidget(parent)
{
    QHBoxLayout* mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(10, 5, 10, 5);

    if (type == MessageBubble::Self) {
        mainLayout->addStretch();
        mainLayout->addWidget(new MessageBubble(sender, message, time, type, this));
        mainLayout->addWidget(new AvatarWidget(sender, this));
    } else {
        mainLayout->addWidget(new AvatarWidget(sender, this));
        mainLayout->addWidget(new MessageBubble(sender, message, time, type, this));
        mainLayout->addStretch();
    }
}

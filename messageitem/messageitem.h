#ifndef MESSAGEITEM_H
#define MESSAGEITEM_H

#include <QWidget>
#include "messagebubble/messagebubble.h"

class MessageItem : public QWidget {
    Q_OBJECT
public:
    explicit MessageItem(const QString& sender, const QString& message,
                         const QDateTime& time, MessageBubble::BubbleType type,
                         QWidget* parent = nullptr);
};

#endif // MESSAGEITEM_H

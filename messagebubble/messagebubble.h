#ifndef MESSAGEBUBBLE_H
#define MESSAGEBUBBLE_H

#include <QFrame>
#include <QDateTime>

class MessageBubble : public QFrame {
    Q_OBJECT
public:
    enum BubbleType { Self, Other };

    explicit MessageBubble(const QString& sender, const QString& message,
                           const QDateTime& time, BubbleType type, QWidget* parent = nullptr);
};

#endif // MESSAGEBUBBLE_H

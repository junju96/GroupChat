#ifndef AVATARWIDGET_H
#define AVATARWIDGET_H

#include <QLabel>

class AvatarWidget : public QLabel {
    Q_OBJECT
public:
    explicit AvatarWidget(const QString& name, QWidget* parent = nullptr);

private:
    QColor generateColor(const QString& name);
};

#endif // AVATARWIDGET_H

#include "avatarwidget.h"
#include <QRandomGenerator>

AvatarWidget::AvatarWidget(const QString& name, QWidget* parent)
    : QLabel(parent)
{
    setFixedSize(40, 40);
    setScaledContents(true);
    setObjectName("avatar");

    QColor bgColor = generateColor(name);
    setStyleSheet(QString("QLabel#avatar { background-color: %1; border-radius: 20px; color: white; font-weight: bold; }")
                  .arg(bgColor.name()));

    setText(name.left(1));
    setStyleSheet(QString("QLabel#avatar { background-color: %1; border-radius: 20px; color: white; font-weight: bold; font-size: 16px; }")
                  .arg(bgColor.name())); // 增大字体大小
    setAlignment(Qt::AlignCenter);
}

QColor AvatarWidget::generateColor(const QString& name)
{
    uint hash = qHash(name);
    return QColor::fromHsv((hash % 360), 200, 230);
}

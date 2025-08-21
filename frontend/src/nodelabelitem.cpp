#include "nodelabelitem.h"
#include <QFont>

NodeLabelItem::NodeLabelItem(const QString &text, QGraphicsItem *parent)
        : QGraphicsTextItem(text, parent)
{
    setFont(QFont("Arial", 10, QFont::Bold));
    setDefaultTextColor(Qt::blue);
}
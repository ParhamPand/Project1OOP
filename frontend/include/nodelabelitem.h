#ifndef NODELABELITEM_H
#define NODELABELITEM_H

#include <QGraphicsTextItem>

class NodeLabelItem : public QGraphicsTextItem
{
public:
    explicit NodeLabelItem(const QString &text, QGraphicsItem *parent = nullptr);

protected:
    // می‌توانید بعداً این تابع را برای استایل‌دهی بهتر override کنید
    // void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // NODELABELITEM_H
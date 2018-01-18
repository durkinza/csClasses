#ifndef HELP_H
#define HELP_H

#include <QGraphicsScene>
#include <QGraphicsTextItem>

class Help: public QGraphicsScene{
public:
    Help();
    void keyPressEvent(QKeyEvent * event);
private:
    QGraphicsTextItem * helpText;
};

#endif // HELP_H

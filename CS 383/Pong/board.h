#ifndef BOARD_H
#define BOARD_H

#include <QGraphicsScene>
#include <ball.h>

class Board: public QGraphicsScene{
public:
    Board();
    void keyPressEvent(QKeyEvent * event);
    void removeBall();
private:
    bool hasBall;
    Ball * ball;
};

#endif // BOARD_H

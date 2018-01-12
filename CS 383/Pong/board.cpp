#include "board.h"
#include <QKeyEvent>
Board::Board()
{
    this->hasBall = false;
}

void Board::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space){
        if(!this->hasBall){
            this->hasBall = true;
            this->ball = new Ball(this->width(), this->height());
            ball->setPos((this->width()/ 2), (this->height()/2));
            this->addItem(ball);
        }
    }else if(event->key() == Qt::Key_Escape){
        if(this->hasBall){
            this->removeItem(this->ball);
            delete this->ball;
            this->hasBall = false;
        }
    }else{
        QGraphicsScene::keyPressEvent(event);
    }
}

void Board::removeBall()
{
    this->hasBall = false;
    //removeItem(this->ball);
}

#include "paddle.h"
#include <game.h>
extern Game * game; // get global game
Paddle::Paddle(bool computer)
{
    width = 20;
    height = 100;
    setRect(0,0,width, height);
    this->computer = computer;
    if(computer){
        //connect to timer
        QTimer * timer = new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(autoMove()));
        timer->start(100);
    }
}

void Paddle::keyPressEvent(QKeyEvent *event)
{
    //qDebug() << "Key was pressed";
    if(event->key() == Qt::Key_Up || event->key() == Qt::Key_W){
        this->move(-10);
    }
    else if(event->key() == Qt::Key_Down || event->key() == Qt::Key_S){
        this->move(10);
    }
}

void Paddle::focusOutEvent()
{
    if(!this->computer){
        this->setFocus();
    }
}

void Paddle::move(int d)
{
    if( (y()+d > 0) && (y()+d+height < game->screen_height)){
        setPos(x(), y()+d);
    }
}

void Paddle::autoMove()
{
    float y = (float(rand())/float(RAND_MAX))*10 + 30;
    if(float(rand())/float(RAND_MAX) > .5){
        move(y);
    }else{
        move(-y);
    }
}

#include "ball.h"

#include <game.h>

extern Game * game; // get global game class

Ball::Ball(int screen_width,int screen_height)
{
    // build ball, set width, height, and starting location
    setRect(0, 0, 10, 10);
    SH = screen_height;
    SW = screen_width;
    MH = RandomFloat(6, 3);
    MV = RandomFloat(float(0.1), float(5));

    //connect to timer to run move function
    QTimer * timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(30);

    // add color to the ball
    //setPen(QPen(Qt::darkBlue, 1));
    setBrush(QBrush(QColor(rand()%255,rand()%255,rand()%255)));
}

void Ball::Bounce(float c)
{

    MV = MV + ((float(rand())/float(RAND_MAX))*c);
    MH = -(MH + (float(rand())/float(RAND_MAX))*5);
}

float Ball::getY()
{
    return float(y());
}


void Ball::move()
{
    QList<QGraphicsItem *> colliding_items = collidingItems();
    for (int i = 0; i < colliding_items.size(); ++i){
        if(typeid(*(colliding_items[i])) == typeid(Paddle)){
            this->Bounce(1);
        }
    }
    qreal a = x();
    qreal b = y();
    if( (pos().y() < 0) || (pos().y()+10 > this->SH)){
        MV = -MV;
    }
    if( (pos().x() < 0) || (pos().x() > this->SW)){
        // increase score
        if(pos().x() > this->SW){
            game->Lscore->increase();

        }else{
            game->Rscore->increase();
        }
        setPos(1,1);
        scene()->removeItem(this);
        game->scene->removeBall();
        delete this;
        //game->scene->removeBall();

        return;
    }
    a= a + MH;
    b = b + MV;
    setPos(a, b);
}

float Ball::RandomFloat(float Max, float Min)
{
    float ret =((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
    if((float(rand())/float(RAND_MAX)) >= 0.5){
        return ret;
    }else{
        return -ret;
    }
}

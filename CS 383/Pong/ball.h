#ifndef BALL_H
#define BALL_H

#include <QTimer>
#include <QGraphicsRectItem>
#include <QObject>
#include <paddle.h>

class Ball: public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Ball(int screen_width, int screen_height);
    void Bounce(float c);
    float getY();
public slots:
    void move();
private:
    int SH; // Screen Height
    int SW; // Screen Width
    float MH; // Move Horizontal
    float MV; // Move Vertical
    float RandomFloat(float Max, float Min);
};

#endif // BALL_H

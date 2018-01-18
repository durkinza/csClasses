#ifndef PADDLE_H
#define PADDLE_H
#include <QTimer>
#include <QKeyEvent>
#include <QGraphicsRectItem>
#include <QObject>

class Paddle: public QObject, public QGraphicsRectItem{
    Q_OBJECT
public:
    Paddle(bool computer);
    void keyPressEvent(QKeyEvent * event);
    void focusOutEvent();
    void move(int d);
public slots:
    void autoMove();
private:
    bool computer;
    int width;
    int height;
};

#endif // PADDLE_H

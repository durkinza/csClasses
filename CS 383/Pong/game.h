#ifndef GAME_H
#define GAME_H

#include "game.h"
#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <help.h>
#include <paddle.h>
#include <ball.h>
#include <board.h>
#include <score.h>

class Game: public QGraphicsView
{
public:
    Game(QWidget * parent=0);
    void keyPressEvent(QKeyEvent * event);
    int screen_width;
    int screen_height;
    Board * scene;
    Help * help;
    QGraphicsTextItem * smallHelp;
    Paddle * RPaddle;
    Paddle * LPaddle;
    Score * Lscore;
    Score * Rscore;
private:
    bool helpShowing;
};

#endif // GAME_H

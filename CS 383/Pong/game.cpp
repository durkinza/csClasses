#include "game.h"

Game::Game(QWidget * parent): QGraphicsView(parent)
{
    screen_height = 600;
    screen_width = 850;
    // Add paddles
    Paddle * RPaddle = new Paddle(false);
    Paddle * LPaddle = new Paddle(true);
    RPaddle->setPos(screen_width - 50, screen_height / 2);
    LPaddle->setPos(0, screen_height / 2);

    // add some scores
    Lscore = new Score();
    Lscore->setPos(screen_width/2 - 100,0);
    Rscore = new Score();
    Rscore->setPos(screen_width/2, 0);
    // Create a scene
    scene = new Board();
    setScene(scene);

    // Add items to scene
    scene->addItem(RPaddle);
    scene->addItem(LPaddle);
    scene->addItem(Lscore);
    scene->addItem(Rscore);

    // Make Right Paddle focusable
    RPaddle->setFlag(QGraphicsItem::ItemIsFocusable);
    RPaddle->setFocus();

    // Set View
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    setFixedSize(screen_width, screen_height);
    scene->setSceneRect(-10,0, screen_width, screen_height);



    show();


}

void Game::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_H){

    }else{
        QGraphicsView::keyPressEvent(event);
    }

}

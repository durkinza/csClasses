#include "game.h"
#include <QtDebug>
Game::Game(QWidget * parent): QGraphicsView(parent)
{
    // Set starting window size (used to set paddle placment too)
    screen_height = 600;
    screen_width = 850;

    // Add paddles
    Paddle * RPaddle = new Paddle(false);
    Paddle * LPaddle = new Paddle(true);
    RPaddle->setPos(screen_width - 50, screen_height / 2);
    LPaddle->setPos(0, screen_height / 2);

    // Add some scores
    Lscore = new Score();
    Lscore->setPos(screen_width/2 - 100,0);
    Rscore = new Score();
    Rscore->setPos(screen_width/2, 0);

    // Add some basic help text
    smallHelp = new QGraphicsTextItem();
    smallHelp->setPlainText("Press h for help.");
    smallHelp->setPos(screen_width/2 - 50, screen_height - 50);

    // Create a game scene
    scene = new Board();
    setScene(scene);

    // Create a help screen
    help = new Help();
    helpShowing = false;

    // Add items to game scene
    scene->addItem(RPaddle);
    scene->addItem(LPaddle);
    scene->addItem(Lscore);
    scene->addItem(Rscore);
    scene->addItem(smallHelp);

    // Make Right Paddle focusable
    RPaddle->setFlag(QGraphicsItem::ItemIsFocusable);
    RPaddle->setFocus();
    //  Paddle will re-focus on focus-out event

    // Set View
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //setFixedSize(screen_width, screen_height);
    scene->setSceneRect(-10,0, screen_width, screen_height);

    // Show the game
    show();
}

void Game::keyPressEvent(QKeyEvent *event){
    // if the user presses h for help
    if(event->key() == Qt::Key_H){
        if(helpShowing){
            // if help is already showing, then close it
            helpShowing = false;
            setScene(this->scene);
            show();
        }else{
            // if help isn't showing, then show it
            helpShowing = true;
            setScene(this->help);
            show();
        }
    }else{
        // send all other key events through.
        QGraphicsView::keyPressEvent(event);
    }

}

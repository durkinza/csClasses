#include "help.h"
#include <QKeyEvent>
Help::Help()
{
  helpText = new QGraphicsTextItem();
  helpText->setPlainText("               How To Play\nUse the arrow keys or w,s keys to move your paddle\nUse the space bar to launch a new ball.\nUse the Esc key to remove a current ball.\nUse the h key to show and hide this help menu.");
  helpText->setDefaultTextColor(Qt::darkGreen);
  helpText->setFont(QFont("times", 16));

  addItem(helpText);
}

void Help::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Back){

    }
}

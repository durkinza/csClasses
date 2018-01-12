#include "score.h"

Score::Score(QGraphicsItem *parent): QGraphicsTextItem(parent){
    // start score at 0;
    score = 0;
    // draw score
    setPlainText(QString("Score: ")+ QString::number(score));
    setDefaultTextColor(Qt::red);
    setFont(QFont("times", 16));
}

void Score::increase()
{
    score++;
    setPlainText(QString("Score: ")+ QString::number(score));
}

int Score::getScore()
{
    return score;
}

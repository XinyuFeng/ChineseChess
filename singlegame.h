#ifndef SINGLEGAME_H
#define SINGLEGAME_H

#include "Board.h"

class SingleGame : public Board
{
    Q_OBJECT
public:
    SingleGame()
    {
        _level = 4;
    }

    virtual void click(int id, int row, int col);

    Step* getBestMove();
    void getAllPossibleMove(QVector<Step *> &steps);

    int getMinScore(int level, int curMaxScore);
    int getMaxScore(int level, int curMinScore);

    void fakeMove(Step* step);
    void unfakeMove(Step* step);
    int calcScore();

    int _level;

public slots:
    void computerMove();


};

#endif // SINGLEGAME_H

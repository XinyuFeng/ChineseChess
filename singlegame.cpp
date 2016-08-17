#include "singlegame.h"
#include <QTimer>

void SingleGame::click(int id, int row, int col)
{
    if(!this->_bRedTurn)
        return;
    Board::click(id, row, col);

    if(!this->_bRedTurn)
    {
        //start a timer
        QTimer::singleShot(100, this, SLOT(computerMove()));

    }
}

void SingleGame::computerMove()
{
    Step* step = getBestMove();
    moveStone(step->_moveid, step->_killid, step->_rowTo, step->_colTo);
    delete step;
    update();
}

void SingleGame::getAllPossibleMove(QVector<Step *> &steps)
{
    int min = 16, max = 32;
    if(this->_bRedTurn)
    {
        min = 0, max = 16;
    }
    for(int i=min; i<max; i++)
    {
        if(_s[i]._dead) continue;

        for(int row=0; row<=9; row++)
        {
            for(int col=0; col<=8; ++col)
            {
                int killid = -1;
                int check = this->hasStone(row, col, killid);
                if(sameColor(killid, i))  continue;

                if(canMove(i, row, col, killid))
                {
                    saveStep(i, killid, row, col, steps);
                }
            }
        }
    }
}

Step* SingleGame::getBestMove()
{
    /*1 which step can move
     * 2 try move
     * 3 evaluate the result of move
     * 4 use the best step as a reference
     */
    //which step can move
    QVector<Step*> steps;
    getAllPossibleMove(steps);

    //try move
    int maxScore = -100000;
    Step* ret = NULL;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();
        fakeMove(step);

        int score = getMinScore(_level - 1, maxScore);
        unfakeMove(step);
        if (score > maxScore)
        {
            maxScore = score;
            if(ret) delete ret;
            ret = step;
        }
        else
        {
            delete step;
        }
    }

    return ret;

}

int SingleGame::getMinScore(int level, int curMaxScore)
{
    if(level == 0) return calcScore();
    QVector<Step*> steps;
    getAllPossibleMove(steps); // the possible move of red stone

    int minScore = 100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();
        fakeMove(step);

        int score = getMaxScore(level - 1, minScore);
        unfakeMove(step);
        delete step;

        if(score <= curMaxScore)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }
        if(score < minScore)
        {
            minScore = score;
        }

    }
    return minScore;
}

int SingleGame::getMaxScore(int level, int curMinScore)
{
    if(level == 0) return calcScore();
    QVector<Step*> steps;
    getAllPossibleMove(steps); // the possible move of red stone

    int maxScore = -100000;
    while(steps.count())
    {
        Step* step = steps.back();
        steps.removeLast();
        fakeMove(step);

        int score = getMinScore(level - 1, maxScore);
        unfakeMove(step);
        delete step;

        if(score >= curMinScore)
        {
            while(steps.count())
            {
                Step* step = steps.back();
                steps.removeLast();
                delete step;
            }
            return score;
        }
        if(score > maxScore)
        {
            maxScore = score;
        }

    }
    return maxScore;
}

void SingleGame::fakeMove(Step *step)
{
    killStone(step->_killid);
    moveStone(step->_moveid, step->_rowTo, step->_colTo);
}

void SingleGame::unfakeMove(Step *step)
{
    reliveStone(step->_killid);
    moveStone(step->_moveid, step->_rowFrom, step->_colFrom);
}

int SingleGame::calcScore()
{
    int redTotalScore = 0;
    int blackTotalScore = 0;
    //enum TYPE{CHE, MA, PAO, BING, JIANG, SHI, XIANG};
    static int chessScore[] = {100,50,50,20,1500,10,10};

    //score black minus socre red;
    for(int i=0; i<16; i++)
    {
        if(_s[i]._dead) continue;
        redTotalScore += chessScore[_s[i]._type];

    }

    for(int i=16; i<32; i++)
    {
        if(_s[i]._dead) continue;
        blackTotalScore += chessScore[_s[i]._type];

    }

    return blackTotalScore - redTotalScore;
}

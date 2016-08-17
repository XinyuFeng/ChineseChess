#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include "stone.h"
#include <QVector>
#include "Step.h"

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget *parent = 0);

    Stone _s[32];
    int _r; /*radius*/

    QPoint _off;
    bool _bSide;

    QVector<Step*> _steps;

    /*game status*/
    int _selectid;
    bool _bRedTurn;
    void init(bool bRedSide);

    QPoint center(int row, int col); //return the corresponding pixel coordinator of chess row and col
    QPoint center(int id);
    bool getRowCol(QPoint pt, int& row, int& col);

    bool findCorrectPos(QPoint pt, int& val);

    void drawStone(QPainter& painter, int id);

    void paintEvent(QPaintEvent *);

    /*important function*/
    bool sameColor(int id1, int id2);
    void moveStone(int moveid, int row, int col);
    bool red(int id);
    void killStone(int id);
    void reliveStone(int id);

    /*move stone*/
    void mouseReleaseEvent(QMouseEvent *);
    void click(QPoint pt);
    virtual void click(int id, int row, int col);
    void trySelectStone(int id);
    void tryMoveStone(int killid, int row, int col);
    void moveStone(int moveid, int killid, int row, int col);
    void saveStep(int moveid, int killid, int row, int col, QVector<Step*>& steps);


    /*rule*/
    bool canMove(int moveid, int row, int col, int clickid);
    bool canMove1(int moveid, int row, int col, int clickid);
    bool canMove2(int moveid, int row, int col, int clickid);
    bool canMove3(int moveid, int row, int col, int clickid);
    bool canMove4(int moveid, int row, int col, int clickid);
    bool canMove5(int moveid, int row, int col, int clickid);
    bool canMove6(int moveid, int row, int col, int clickid);
    bool canMove7(int moveid, int row, int col, int clickid);

    int getStoneCountAtLine(int row1, int col1, int row2, int col2);

    bool hasStone(int row, int col, int& id);
    bool canSelect(int id);

    /*rule helper*/
    bool isBottomSide(int id);

    ~Board();

signals:

public slots:
};

#endif // BOARD_H

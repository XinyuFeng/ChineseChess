#include "board.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#define GetRowCol(__row, __col, __id) \
    int __row = _s[__id]._row;\
    int __col = _s[__id]._col


Board::Board(QWidget *parent) : QWidget(parent)
{
    _r = 20;
    QSize _size(_r*21, _r*22);
    this->setFixedSize(_size);
    //setMinimumSize(_r*18+4, _r*20+4);
    init(true);

}

void Board::init(bool bRedSide)
{
    for(int i=0; i<32; i++)
    {
        _s[i].init(i);
    }
    _selectid = -1;
    _bRedTurn = true;
    _bSide = bRedSide;

    if(bRedSide)
    {
        for(int i=0; i<32; ++i)
        {
            _s[i].rotate();
        }
    }
}

Board::~Board()
{

}

void Board::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int d = 40;
    _r = d/2;
    //draw 10 lines
    for(int i=1; i<=10; i++)
    {
        painter.drawLine(QPoint(d, i*d), QPoint(9*d, i*d));
    }
    //draw 9 lines
    for(int i=1; i<=9; i++)
    {
        if(i==1 || i==9)
            painter.drawLine(QPoint(i*d, d), QPoint(i*d, 10*d));
        else
        {
            painter.drawLine(QPoint(i*d, d), QPoint(i*d, 5*d));
            painter.drawLine(QPoint(i*d, 6*d), QPoint(i*d, 10*d));



        }

    }

    //nine grid
    painter.drawLine(QPoint(4*d, d), QPoint(6*d, 3*d));
    painter.drawLine(QPoint(6*d, d), QPoint(4*d, 3*d));

    painter.drawLine(QPoint(4*d, 8*d), QPoint(6*d, 10*d));
    painter.drawLine(QPoint(6*d, 8*d), QPoint(4*d, 10*d));


    //paint 32 stones
    for(int i=0; i<32; i++)
    {
        drawStone(painter, i);
    }



}

void Board::drawStone(QPainter& painter, int id)
{
    if(_s[id]._dead)
        return;


    QPoint c = center(id);
    QRect rect = QRect(c.x()-_r, c.y()-_r, _r*2, _r*2);

    if(id == _selectid)
        painter.setBrush(QBrush(Qt::gray));
    else
        painter.setBrush(QBrush(Qt::yellow));

    painter.setPen(Qt::black);

    painter.drawEllipse(center(id), _r, _r);

    if(_s[id]._red)
        painter.setPen(Qt::red);

    painter.setFont(QFont("system", _r, 700));


    painter.drawText(rect, _s[id].getText(), QTextOption(Qt::AlignCenter));
}

QPoint Board::center(int row, int col)
{
    QPoint ret;
    ret.rx() = (col+1)* _r*2;
    ret.ry() = (row+1)* _r*2;
    return ret;
}

QPoint Board::center(int id)
{
    return center(_s[id]._row, _s[id]._col);
}

//need improve
bool Board::getRowCol(QPoint pt, int &row, int &col)
{

    if(pt.x()>=_r && pt.x()<=19*_r && pt.y()>=_r && pt.y()<=21*_r) //make sure the pt is within the range of the board.
    {
        int rightX = -1;
        int rightY = -1;

        if(pt.x()%(2*_r)>_r)                     //get the nearest crossing point(rightX, rightY) on the board.
            rightX = pt.x()-pt.x()%_r+_r;
        else
            rightX = pt.x()-pt.x()%(2*_r);

        if(pt.y()%(2*_r)>_r)
            rightY = pt.y()-pt.y()%_r+_r;
        else
            rightY = pt.y()-pt.y()%(2*_r);

        int dx = pt.x() - rightX;
        int dy = pt.y() - rightY;
        int dist = dx*dx+dy*dy;
        if(dist < _r*_r)                       //checking the distance between pt and (rightX, rightY).
        {
            col = rightX/(2*_r) - 1;
            row = rightY/(2*_r) - 1;
            return true;
        }
    }





    return false;
}

void Board::mouseReleaseEvent(QMouseEvent *ev)
{
    if(ev->button() != Qt::LeftButton)
    {
        return;
    }

    click(ev->pos());

}

int Board::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
    int ret = 0;
    int id = -1;
        if(row1 != row2 && col1 != col2)
            return -1;
        if(row1 == row2 && col1 == col2)
            return -1;

        if(row1 == row2)
        {
            int min = col1 < col2 ? col1 : col2;
            int max = col1 < col2 ? col2 : col1;
            for(int col = min+1; col<max; ++col)
            {
                if(hasStone(row1, col, id)) ++ret;
            }
        }
        else
        {
            int min = row1 < row2 ? row1 : row2;
            int max = row1 < row2 ? row2 : row1;
            for(int row = min+1; row<max; ++row)
            {
                if(hasStone(row, col1, id)) ++ret;
            }
        }

        return ret;
}

bool Board::hasStone(int row, int col, int& id)
{
    for(int i=0; i<32; i++)
    {
        if(_s[i]._row == row && _s[i]._col == col && _s[i]._dead == false)
        {
            id = i;
            return true;
        }

    }
    return false;
}

bool Board::isBottomSide(int id)
{
    return _bSide == _s[id]._red;
}

bool Board::canMove1(int moveid, int row, int col, int clickid)
{
    //1. must be in the nine grids
    //2. move only one grid
    if(clickid != -1 && _s[clickid]._type == Stone::JIANG)
        return canMove5(moveid, row, col, clickid);

    if(!isBottomSide(moveid))
    {
        if(row > 2) return false;
    }
    else
    {
        if(row < 7) return false;
    }

    if(col < 3 || col > 5)
        return false;

    int dr = _s[moveid]._row-row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 1 || d == 10)
        return true;


    return false;
}

bool Board::canMove2(int moveid, int row, int col, int clickid)
{
    if(!isBottomSide(moveid))
    {
        if(row > 2) return false;
    }
    else
    {
        if(row < 7) return false;
    }

    if(col < 3 || col > 5)
        return false;

    int dr = _s[moveid]._row-row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    if(d == 11)
        return true;

    return false;
}

bool Board::canMove3(int moveid, int row, int col, int clickid)
{
    if(!isBottomSide(moveid))
    {
        if(row > 4) return false;
    }
    else
    {
        if(row < 5) return false;
    }

    int dr = _s[moveid]._row-row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    int posX = (row+_s[moveid]._row)/2;
    int posY = (col+_s[moveid]._col)/2;

    int i = -1;
    if(d == 22 && !hasStone(posX, posY, i))
    {

        return true;
    }

    return false;
}

bool Board::canMove4(int moveid, int row, int col, int clickid)
{
    int dr = _s[moveid]._row-row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);
    int posX = (row+_s[moveid]._row)/2;
    int posY = (col+_s[moveid]._col)/2;
    int i;
    if(d == 21)
    {
        if(!hasStone(posX, _s[moveid]._col,i))
            return true;
    }
    else if(d == 12)
    {
        if(!hasStone(_s[moveid]._row, posY, i))
            return true;
    }

    return false;
}

bool Board::canMove5(int moveid, int row, int col, int clickid)
{
    int ret = getStoneCountAtLine(_s[moveid]._row, _s[moveid]._col, row, col);
        if(ret == 0)
            return true;
        return false;
}

bool Board::canMove6(int moveid, int row, int col, int clickid)
{
    int ret = getStoneCountAtLine(_s[moveid]._row, _s[moveid]._col, row, col);
        if(clickid != -1)
        {
            if(ret == 1) return true;
        }
        else
        {
            if(ret == 0) return true;
        }
        return false;
}

bool Board::canMove7(int moveid, int row, int col, int clickid)
{
    int dr = _s[moveid]._row-row;
    int dc = _s[moveid]._col - col;
    int d = abs(dr)*10 + abs(dc);

    if(d != 1 && d != 10) return false;

    if(!isBottomSide(moveid))
    {
        if(_s[moveid]._row > row) return false;
        if(_s[moveid]._row == row && _s[moveid]._row <= 4) return false;
    }
    else
    {
        if(_s[moveid]._row < row) return false;
        if(_s[moveid]._row == row && _s[moveid]._row >= 5) return false;
    }

    return true;

}

bool Board::canMove(int moveid, int row, int col, int clickid)
{
    if(sameColor(moveid, clickid))
    {
        return false;
    }

    switch(_s[moveid]._type)
    {
    case Stone::JIANG:
        return canMove1(moveid, row, col, clickid);
        break;
    case Stone::SHI:
        return canMove2(moveid, row, col, clickid);
        break;
    case Stone::XIANG:
        return canMove3(moveid, row, col, clickid);
        break;
    case Stone::MA:
        return canMove4(moveid, row, col, clickid);
        break;
    case Stone::CHE:
        return canMove5(moveid, row, col, clickid);
        break;
    case Stone::PAO:
        return canMove6(moveid, row, col, clickid);
        break;
    case Stone::BING:
        return canMove7(moveid, row, col, clickid);
        break;
    }

    return true;
}

void Board::click(int id, int row, int col)
{
    if(this->_selectid == -1)
    {
        trySelectStone(id);
    }
    else
    {
        tryMoveStone(id, row, col);
    }
}

void Board::click(QPoint pt)
{
    int row, col;
    bool bClicked = getRowCol(pt, row, col);
    if(!bClicked) return;

    int id = -1;
    int check = hasStone(row, col, id);
    click(id, row, col);
}

bool Board::canSelect(int id)
{
    return _bRedTurn == _s[id]._red;
}

void Board::trySelectStone(int id)
{
    if(id == -1)
        return;

    if(!canSelect(id)) return;

    _selectid = id;
    update();
}

void Board::tryMoveStone(int killid, int row, int col)
{
    if(killid != -1 &&sameColor(killid, _selectid))
    {
        trySelectStone(killid);
        return;
    }

    bool ret = canMove(_selectid, row, col, killid);
    if(ret)
    {
        moveStone(_selectid, killid, row, col);
        _selectid = -1;
        update();
    }
}

bool Board::sameColor(int id1, int id2)
{
    if(id1 == -1 || id2 == -1) return false;

    return red(id1) == red(id2);
}

bool Board::red(int id)
{
    return _s[id]._red;
}

void Board::moveStone(int moveid, int killid, int row, int col)
{
//    saveStep(moveid, killid, row, col, _steps);

    killStone(killid);
    moveStone(moveid, row, col);
}

void Board::killStone(int id)
{
    if(id==-1) return;
    _s[id]._dead = true;
}

void Board::moveStone(int moveid, int row, int col)
{
    _s[moveid]._row = row;
    _s[moveid]._col = col;

    _bRedTurn = !_bRedTurn;
}

void Board::saveStep(int moveid, int killid, int row, int col, QVector<Step *> &steps)
{
    GetRowCol(row1, col1, moveid);
    Step* step = new Step;
    step->_colFrom = col1;
    step->_colTo = col;
    step->_rowFrom = row1;
    step->_rowTo = row;
    step->_moveid = moveid;
    step->_killid = killid;

    steps.append(step);
}

void Board::reliveStone(int id)
{
    if(id == -1) return;
    _s[id]._dead = false;

}

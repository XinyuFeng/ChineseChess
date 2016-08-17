#ifndef NETGAME_H
#define NETGAME_H

#include "board.h"
#include <QTcpServer>
#include <QTcpSocket>

/*
 * red or black, sent by server, received by client
 first byte is 1, second byte is 1 or 0, 1: receiver takes red, 0: receiver takes black
 * send click information(coordination, id)
 first byte is 2, second byte is row, third byte is col, fourth byte is clickid;
 */

class NetGame : public Board
{
    Q_OBJECT
public:
    NetGame(bool server);
    ~NetGame();

    QTcpServer* _server;
    QTcpSocket* _socket;

    void click(int id, int row, int col);

public slots:
    void slotNewConnection();
    void slotRecv();
};

#endif // NETGAME_H

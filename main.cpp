#include <QApplication>
#include "singlegame.h"
#include "netgame.h"
#include <QMessageBox>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QMessageBox::StandardButton ret;
    ret = QMessageBox::question(NULL, "server or client", "server");

    bool bServer = false;
    if(ret == QMessageBox::Yes)
    {
        bServer = true;
    }

    NetGame board(bServer);
    board.show();
    return app.exec();
}

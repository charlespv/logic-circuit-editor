//
// Created by charles on 08/12/16.
//

#include  <QApplication>
#include  <QtGui>
#include "CellViewer.h"
#include "Cell.h"
using namespace Netlist;


int  main (int argc , char* argv []) {
    QApplication *qa = new QApplication(argc, argv);
    Cell* halfadder = Cell::load("halfadder");
    halfadder->save();
    CellViewer *viewer = new CellViewer();
    viewer->setCell(halfadder);
    viewer->show();

    int rvalue = qa->exec();
    delete qa;
    return rvalue;
}


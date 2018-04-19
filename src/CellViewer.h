#ifndef NETLIST_CELLVIEWER_H
#define NETLIST_CELLVIEWER_H

#include <QMainWindow>
#include <QObject>
#include "Cell.h"
#include "CellWidget.h"
#include "SaveCellDialog.h"
#include "InstanceWidget.h"
#include "CellsLib.h"
#include "OpenCellDialog.h"
#include "NetsWidget.h"

namespace Netlist {
    class CellsLib;

    class CellViewer : public QMainWindow {
    Q_OBJECT;
    public:
        CellViewer(QWidget *parent = NULL);
        virtual    ~CellViewer();
        void setCell(Cell *);
        Cell *getCell() const;
        inline CellsLib* getCellsLib() const;

    public slots:
        //void setCell(Cell *);
        void saveCell();
        void openCell();
        void showCellsLib();
        void showInstanceWidget();
        void showIconViewer();
        //void showNetsWidget();
        void showQuitDialog();
        void showMessageBox();
        void closeEvent(QCloseEvent*);
    signals:
        void cellLoaded();
    private :
        CellWidget *cellWidget_;
        SaveCellDialog *saveCellDialog_;
        CellWidget* iconWidget_;
        CellsLib* cellsLib_;
        InstanceWidget *instanceWidget_;

        //SaveCellWidget *SaveCellWidget_;
        NetsWidget* netsWidget_;

    };


    inline CellsLib* CellViewer::getCellsLib() const {return cellsLib_;}
} //Netlist namespace
#endif   // NETLIST_CELLVIEWER_H
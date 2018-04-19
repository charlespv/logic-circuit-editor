#ifndef NETLIST_CELLS_LIB_H
#define NETLIST_CELLS_LIB_H

#include <QTableView>
#include <QWidget>
#include "CellsModel.h"

namespace Netlist
{
    class CellViewer;
    class CellsLib: public QWidget
    {
    Q_OBJECT
    public:
        CellsLib(QWidget* parent = NULL);
        void setCellViewer(CellViewer*);
        int getSelectedRow() const;
        inline CellsModel* getBaseModel() const;
    public slots:
        void load();
        void load(const QModelIndex&);
// Pour fermer la fenêtre après sélection de Cell...
//		signals:
//			void cellLoaded();
    private:
        CellViewer* cellViewer_;
        CellsModel* baseModel_;
        QTableView* view_;
        //QPushButton* load_;
    };

    inline CellsModel* CellsLib::getBaseModel() const { return baseModel_; }
} // Netlist namespace.

#endif // NETLIST_CELLS_LIB_H

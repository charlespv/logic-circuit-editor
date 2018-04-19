#ifndef NETLIST_CELLS_MODEL_H
#define NETLIST_CELLS_MODEL_H

#include <QAbstractTableModel>
#include <vector>
#include "Cell.h"

namespace Netlist
{
    class CellsModel: public QAbstractTableModel
    {
    Q_OBJECT
    public:
        CellsModel(QObject* parent = NULL);
        void setCells(std::vector<Cell*> cells);
        Cell* getModel(int row);
        int rowCount(const QModelIndex& parent = QModelIndex()) const;
        int columnCount(const QModelIndex& parent = QModelIndex()) const;
        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    public slots:
        void updateDatas();
    private:
        std::vector<Cell*> cells_;
    };
} // Netlist namespace.

#endif // NETLIST_CELLS_MODEL_H
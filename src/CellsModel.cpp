#include "CellsModel.h"

namespace Netlist
{
    CellsModel::CellsModel(QObject* parent)
            : QAbstractTableModel(parent)
            , cells_(NULL)
    {}

    void CellsModel::setCells(std::vector<Cell*> cells)
    {
        emit layoutAboutToBeChanged();
        cells_ = cells;
        emit layoutChanged();
    }

    Cell* CellsModel::getModel(int row)
    {
        if ((cells_.empty()) or (row >= (int)cells_.size()))
            return NULL;
        return cells_[row];
    }

    int CellsModel::rowCount(const QModelIndex& parent) const
    {
        return cells_.size();
    }

    int CellsModel::columnCount(const QModelIndex& parent) const
    {
        return 1;
    }

    QVariant CellsModel::data(const QModelIndex& index, int role) const
    {
        if ((cells_.empty()) or (not index.isValid()) or (role != Qt::DisplayRole))
            return QVariant();
        return cells_[index.row()]->getName().c_str();
    }

    QVariant CellsModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if ((orientation == Qt::Vertical) or (role != Qt::DisplayRole))
            return QVariant();
        return (section == 0) ? "Name" : QVariant();
    }

    void CellsModel::updateDatas()
    {
        setCells(Cell::getAllCells());
    }
} // Netlist namespace.
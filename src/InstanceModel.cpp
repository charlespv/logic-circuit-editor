#include <iostream>
#include "InstanceModel.h"
#include "Instance.h"

namespace Netlist
{
    InstanceModel::InstanceModel(QObject* parent)
            : QAbstractTableModel(parent)
            , cell_(NULL)
    {}

    //InstanceModel::~InstanceModel() {}

    void InstanceModel::setCell(Cell* cell)
    {
        emit layoutAboutToBeChanged();
        cell_ = cell;
        emit layoutChanged();
    }

    Cell* InstanceModel::getModel(int row)
    {
        if ((not cell_) or (row >= (int)cell_->getInstances().size()))
            return NULL;
        return cell_->getInstances()[row]->getMasterCell();
    }

    int InstanceModel::rowCount(const QModelIndex& parent) const
    {
        return (cell_) ? cell_->getInstances().size() : 0;
    }

    int InstanceModel::columnCount(const QModelIndex& parent) const
    {
        return 2;
    }

    QVariant InstanceModel::data(const QModelIndex& index, int role) const
    {
        if ((not cell_) or (not index.isValid()))
            return QVariant();
        if (role == Qt::DisplayRole)
        {
            int row = index.row();
            switch(index.column())
            {
                case 0:
                    return cell_->getInstances()[row]->getName().c_str();
                case 1:
                    return cell_->getInstances()[row]->getMasterCell()->getName().c_str();
            }
        }
        return QVariant();
    }

    QVariant InstanceModel::headerData(int section, Qt::Orientation orientation, int role) const
    {
        if (orientation == Qt::Vertical)
            return QVariant();
        if (role != Qt::DisplayRole)
            return QVariant();
        switch(section)
        {
            case 0: return "Instance";
            case 1: return "MasterCell";
        }
        return QVariant();
    }
} // Netlist namespace.
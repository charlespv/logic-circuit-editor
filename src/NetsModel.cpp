#include <iostream>
#include "NetsModel.h"
#include "Net.h"

namespace Netlist
{
	NetsModel::NetsModel(QObject* parent)
		: QAbstractTableModel(parent)
		, cell_(NULL)
	{}

	void NetsModel::setCell(Cell* cell)
	{
		emit layoutAboutToBeChanged();
		cell_ = cell;
		emit layoutChanged();
	}

	Cell* NetsModel::getCell() 
	{
		return cell_;
	}

	int NetsModel::rowCount(const QModelIndex& parent) const
	{
		return (cell_) ? cell_->getNets().size() : 0;
	}

	int NetsModel::columnCount(const QModelIndex& parent) const
	{
		return 1;
	}

	QVariant NetsModel::data(const QModelIndex& index, int role) const
	{
		if ((not cell_) or (not index.isValid()) or (role != Qt::DisplayRole))
			return QVariant();
		return cell_->getNets()[index.row()]->getName().c_str();
	}

	QVariant NetsModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (orientation == Qt::Vertical)
			return QVariant();
		if (role != Qt::DisplayRole)
			return QVariant();
		return (section == 0) ? "Net" : QVariant();
	}
} // Netlist namespace.	

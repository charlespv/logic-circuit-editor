#ifndef NETLIST_NETS_MODEL_H
#define NETLIST_NETS_MODEL_H

#include <QAbstractTableModel>
#include "Cell.h"

namespace Netlist
{
	class NetsModel: public QAbstractTableModel
	{
		Q_OBJECT
		public:
			NetsModel(QObject* parent = NULL);
			void setCell(Cell*);
			Cell* getCell();
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			int columnCount(const QModelIndex& parent = QModelIndex()) const;
			QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
			QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
		private:
			Cell* cell_;
	};
} // Netlist namespace.

#endif // NETLIST_NETS_MODEL_H

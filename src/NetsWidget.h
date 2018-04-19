#ifndef NETLIST_NETS_WIDGET_H
#define NETLIST_NETS_WIDGET_H

#include <QWidget>
#include <QTableView>
#include "NetsModel.h"

namespace Netlist
{
	class CellViewer;
	class NetsWidget: public QWidget
	{
		Q_OBJECT
		public:
			NetsWidget(QWidget* parent = NULL);
			void setCellViewer(CellViewer*);
			QModelIndexList getSelectedRows() const;
			inline void setCell(Cell*);
		public slots:
			void view();
// Pour fermer la fenêtre après sélection Net...
		signals:
			void netLoaded();
		private:	
				CellViewer* cellViewer_;
				NetsModel* baseModel_;
				QTableView* view_;
	};

	inline void NetsWidget::setCell(Cell* cell) { baseModel_->setCell(cell); }
} // Netlist namespace.

#endif // NETLIST_NETS_WIDGET_H

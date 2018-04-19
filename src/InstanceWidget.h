#ifndef NETLIST_INSTANCE_WIDGET_H
#define NETLIST_INSTANCE_WIDGET_H

#include <QWidget>
#include <QTableView>
#include "InstanceModel.h"

namespace Netlist
{
    class CellViewer;
    class InstanceWidget: public QWidget
    {
    Q_OBJECT
    public:
        InstanceWidget(QWidget* parent = NULL);
        void setCellViewer(CellViewer*);
        int getSelectedRow() const;
        inline void setCell(Cell*);
    public slots:
        void load();
        void load(const QModelIndex&);
// Pour fermer la fenêtre après élection Instance...
//		signals:
//			void instanceLoaded();
    private:
        CellViewer* cellViewer_;
        InstanceModel* baseModel_;
        QTableView* view_;
        //QPushButton* load_;
    };

    inline void InstanceWidget::setCell(Cell* cell) { baseModel_->setCell(cell); }
} // Netlist namespace.

#endif // NETLIST_INSTANCES_WIDGET_H
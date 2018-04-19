#ifndef NETLIST_OPEN_CELL_DIALOG_H
#define NETLIST_OPEN_CELL_DIALOG_H

#include <QDialog>
#include <QLineEdit>

namespace Netlist
{
    class OpenCellWidget: public QDialog
    {
    Q_OBJECT
    public:
        OpenCellWidget(QWidget* parent = NULL);
        static QString run();
        const QString getCellName() const;
        void setCellName(QString& name);
    protected:
        QLineEdit* lineEdit_;
    };
} // Netlist namespace.

#endif // NETLIST_OPEN_CELL_DIALOG_H

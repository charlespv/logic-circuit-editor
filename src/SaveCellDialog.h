#ifndef NETLIST_SAVE_CELL_DIALOG_H
#define NETLIST_SAVE_CELL_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QObject>

namespace Netlist
{
    class SaveCellDialog: public QDialog
    {
    Q_OBJECT
    public:
                SaveCellDialog(QWidget* parent = NULL);
        bool    run(QString& name);
        const   QString getCellName() const;
        void    setCellName(QString& name);
    protected:
        QLineEdit* lineEdit_;
    };
} // Netlist namespace.

#endif // NETLIST_SAVE_CELL_DIALOG_H
#include "SaveCellDialog.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

namespace Netlist
{
    SaveCellDialog::SaveCellDialog(QWidget* parent)
            : QDialog(parent)
            , lineEdit_(NULL)
    {
        setWindowTitle("Save Cell");
        QLabel* label = new QLabel("Enter cell name:");

        lineEdit_ = new QLineEdit();
        lineEdit_->setMinimumWidth(400);

        QPushButton* okButton = new QPushButton("OK");
        okButton->setFont(QFont("Helvetica", 10, QFont::Bold, true));
        okButton->setCursor(Qt::PointingHandCursor);

        QPushButton* cancelButton = new QPushButton("Cancel");
        cancelButton->setFont(QFont("Helvetica", 10, QFont::Bold, true));
        cancelButton->setCursor(Qt::PointingHandCursor);
        okButton->setDefault(true);

        connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
        connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(okButton);
        buttonLayout->addStretch();
        buttonLayout->addWidget(cancelButton);
        buttonLayout->addStretch();

        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(label);
        mainLayout->addWidget(lineEdit_);
        mainLayout->addLayout(buttonLayout);
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);

        setLayout(mainLayout);

    }

    const QString SaveCellDialog::getCellName() const
    {
        return lineEdit_->text();
    }

    void SaveCellDialog::setCellName(QString& name)
    {
        lineEdit_->setText(name);
    }

    bool SaveCellDialog::run(QString& name)
    {
        setCellName(name);
        int result = exec();
        name = getCellName();
        return (result == Accepted);
    }
} // Netlist namespace.

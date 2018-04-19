#include "OpenCellDialog.h"
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFont>

namespace Netlist
{
    OpenCellWidget::OpenCellWidget(QWidget* parent)
            : QDialog(parent)
            , lineEdit_(NULL)
    {
        QLabel* label = new QLabel("Enter cell name:");
        lineEdit_ = new QLineEdit();
        lineEdit_->setMinimumWidth(400);
        QPushButton* okButton = new QPushButton("OK");
        QPushButton* cancelButton = new QPushButton("Cancel");
        okButton->setFont(QFont("Comic Sans MS", 10, QFont::Bold, true));
        okButton->setCursor(Qt::PointingHandCursor);
        cancelButton->setFont(QFont("Comic Sans MS", 10, QFont::Bold, true));
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
        setWindowTitle("Open Cell");
    }

    const QString OpenCellWidget::getCellName() const
    {
        return lineEdit_->text();
    }

    void OpenCellWidget::setCellName(QString& name)
    {
        lineEdit_->setText(name);
    }

    QString OpenCellWidget::run()
    {
        OpenCellWidget openCellWidget;
        openCellWidget.exec();
        QString name = openCellWidget.getCellName();
        return name;
    }
} // Netlist namespace.

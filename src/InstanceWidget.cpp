#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <iostream>
#include "InstanceWidget.h"
#include "CellViewer.h"

namespace Netlist
{
    InstanceWidget::InstanceWidget(QWidget* parent)
            : QWidget(parent)
            , cellViewer_(NULL)
            , baseModel_(new InstanceModel(this))
            , view_(new QTableView(this))
            //, load_ (new QPushButton(this))
    {
        setAttribute(Qt::WA_QuitOnClose, false);
        setAttribute(Qt::WA_DeleteOnClose, false);
        setContextMenuPolicy(Qt::ActionsContextMenu);

        view_->setShowGrid(false);
        view_->setAlternatingRowColors(true);
        view_->setSelectionBehavior(QAbstractItemView::SelectRows);
        view_->setSelectionMode(QAbstractItemView::SingleSelection);
        view_->setSortingEnabled(true);
        view_->setModel(baseModel_);
        view_->verticalHeader()->hide();

        QHeaderView* hHeader = view_->horizontalHeader();
        hHeader->setDefaultAlignment(Qt::AlignHCenter);
        hHeader->setMinimumSectionSize(300);
        hHeader->setStretchLastSection(true);

        QPushButton* load = new QPushButton("Load");
        load->setFont(QFont("Comic Sans MS", 10, QFont::Bold, true));
        load->setCursor(Qt::PointingHandCursor);
        load->setDefault(true);

        QHBoxLayout* hLayout = new QHBoxLayout();
        hLayout->addStretch();
        hLayout->addWidget(load);
        hLayout->addStretch();

        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addWidget(view_);
        mainLayout->addLayout(hLayout);

        connect(load, SIGNAL(clicked()), this, SLOT(load()));
        connect(view_, SIGNAL(doubleClicked(const QModelIndex&)), this, SLOT(load(const QModelIndex&)));
//		connect(this, SIGNAL(instanceLoaded()), this, SLOT(close()));

        setLayout(mainLayout);
        setWindowTitle("Instances List");
    }

    void InstanceWidget::setCellViewer(CellViewer* cellViewer)
    {
        cellViewer_ = cellViewer;
    }

    int InstanceWidget::getSelectedRow() const
    {
        QModelIndexList selecteds = view_->selectionModel()->selection().indexes();
        if (selecteds.empty())
            return -1;
        return selecteds.first().row();
    }

    void InstanceWidget::load()
    {
        int selectedRow = getSelectedRow();
        if (selectedRow < 0)
            return;
        cellViewer_->setCell(baseModel_->getModel(selectedRow));
        std::cout << "<" << cellViewer_->getCell()->getName() << "> cell loaded\n";
//		emit instanceLoaded();
    }

    void InstanceWidget::load(const QModelIndex& index)
    {
        int row = index.row();
        cellViewer_->setCell(baseModel_->getModel(row));
        std::cout << "<" << cellViewer_->getCell()->getName() << "> cell loaded\n";
//		emit instanceLoaded();
    }
}	// Netlist namespace.
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <iostream>
#include "NetsWidget.h"
#include "CellViewer.h"
#include "Net.h"

namespace Netlist
{
	NetsWidget::NetsWidget(QWidget* parent)
		: QWidget(parent)
		, cellViewer_(NULL)
		, baseModel_(new NetsModel())
		, view_(new QTableView())
	{
		setAttribute(Qt::WA_QuitOnClose, false);
		setAttribute(Qt::WA_DeleteOnClose, false);
		setContextMenuPolicy(Qt::ActionsContextMenu);
		
		view_->setShowGrid(false);
		view_->setAlternatingRowColors(true);
		view_->setSelectionBehavior(QAbstractItemView::SelectRows);
		view_->setSelectionMode(QAbstractItemView::ExtendedSelection);
		view_->setSortingEnabled(true);
		view_->setModel(baseModel_);
		view_->verticalHeader()->hide();
		
		QHeaderView* hHeader = view_->horizontalHeader();
		hHeader->setDefaultAlignment(Qt::AlignHCenter);
		hHeader->setMinimumSectionSize(300);
		hHeader->setStretchLastSection(true);

		QPushButton* view = new QPushButton("View");
		view->setFont(QFont("Comic Sans MS", 10, QFont::Bold, true));
		view->setCursor(Qt::PointingHandCursor);
		view->setDefault(true);

		QHBoxLayout* hLayout = new QHBoxLayout();
		hLayout->addStretch();
		hLayout->addWidget(view);
		hLayout->addStretch();

		QVBoxLayout* mainLayout = new QVBoxLayout();
		mainLayout->addWidget(view_);
		mainLayout->addLayout(hLayout);

		connect(view, SIGNAL(clicked()), this, SLOT(view()));
		connect(this, SIGNAL(netLoaded()), this, SLOT(close()));

		setLayout(mainLayout);
		setWindowTitle("Nets List");
	}	

	void NetsWidget::setCellViewer(CellViewer* cellViewer)
	{
		cellViewer_ = cellViewer;
	}

	QModelIndexList NetsWidget::getSelectedRows() const
	{
		return view_->selectionModel()->selection().indexes();
	}
	
	void NetsWidget::view()
	{
		QModelIndexList selectedRows = getSelectedRows();
		if (selectedRows.empty())
			return;
		for (QModelIndexList::iterator ir = selectedRows.begin(); ir != selectedRows.end(); ++ir)
		{
			//baseModel_->getCell()->getNets()[ir->row()]->setSelected(true);
		}
		repaint();
		emit netLoaded();
	}
}	// Netlist namespace.

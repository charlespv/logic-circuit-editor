#include <QMenu>
#include <QMenuBar>
#include <string>
#include <QAction>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QFont>
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include "Cell.h"
#include "CellViewer.h"


namespace Netlist
{
    CellViewer::CellViewer(QWidget *parent)
        : QMainWindow(parent)
        , cellWidget_ (NULL)
        , iconWidget_ (NULL)
        , instanceWidget_(NULL)
        //, SaveCellWidget_ (NULL)
        , saveCellDialog_(NULL)
        , netsWidget_(NULL)
        , cellsLib_(NULL)
        {

            cellWidget_ = new CellWidget();
            //cellWidget_->setMode(CellWidget::Structurel);
            iconWidget_ = new CellWidget();
            //iconWidget_->setMode(CellWidget::Icon);
            //saveCellWidget_ = new SaveCellWidget();
            saveCellDialog_ = new SaveCellDialog (this);
            setCentralWidget(cellWidget_);
            //cours 8
            QMenu* fileMenu = menuBar()->addMenu("&File");
            QAction* action = new QAction("&Open Cell", this);
            action->setStatusTip(tr("Open a cell"));
            action->setShortcut(QKeySequence(QKeySequence::Open));
            action->setVisible(true);
            fileMenu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(openCell()));

            action = new QAction("&Save As", this);
            action->setStatusTip(tr("Save to disk (rename the cell)"));
            action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
            action->setVisible(true);
            fileMenu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(saveCell()));

            action = new QAction("&Instances", this);
            action->setStatusTip(tr("Show instances list"));
            action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_I));
            action->setVisible(true);
            fileMenu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(showInstanceWidget()));

            action = new QAction("&Cells Lib", this);
            action->setStatusTip(tr("Show cells library"));
            action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
            action->setVisible(true);
            fileMenu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(showCellsLib()));

            action = new QAction("Icon &View", this);
            action->setStatusTip(tr("Show icon viewer"));
            action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
            action->setVisible(true);
            fileMenu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(showIconViewer()));

            action = new QAction("&Nets", this);
            action->setStatusTip(tr("Show nets list"));
            action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
            action->setVisible(true);
            fileMenu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(showNetsWidget()));

            action = new QAction("&Refresh", this);
            action->setStatusTip(tr("Refresh the Netlist Viewer"));
            action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
            action->setVisible(true);
            fileMenu->addAction(action);
            connect(action, SIGNAL(triggered()), cellWidget_, SLOT(refresh()));

            action = new QAction("&Quit", this);
            action->setStatusTip(tr("Exit the Netlist Viewer"));
            action->setShortcut(QKeySequence(QKeySequence::Quit));
            action->setVisible(true);
            fileMenu->addAction(action);
            connect(action, SIGNAL(triggered()), this, SLOT(showQuitDialog()));
        }
    CellViewer::~CellViewer()
    {}

    Cell* CellViewer::getCell() const
    {
        return cellWidget_->getCell();
    }

    void CellViewer::showMessageBox()
    {
        QMessageBox::warning(this, "Warning", "Modele non trouve !");
    }

    void CellViewer::closeEvent(QCloseEvent* event)
    {
        showQuitDialog();
    }

    void CellViewer::openCell()
    {
        Cell* cell = NULL;
        while (cell == NULL)
        {
            std::string cellName = OpenCellWidget::run().toStdString();
            if (cellName.empty())
                return;
            cell = Cell::find(cellName);
            if (!cell)
            {
                std::cout << "Chargement du modele:" << std::endl;
                std::cout << "-<" << cellName << "> ..." << std::endl;
                cell = Cell::load(cellName);
                if (!cell)
                {
                    showMessageBox();
                    std::cout << "Modele <" << cellName << "> non trouve..." << std::endl;
                }
            }
        }
        setCell(cell);
        emit cellLoaded();
    }

    void CellViewer::showCellsLib()
    {
        cellsLib_ = new CellsLib();
        connect(this, SIGNAL(cellLoaded()), cellsLib_->getBaseModel(), SLOT(updateDatas()));
        cellsLib_->setCellViewer(this);
        cellsLib_->show();
    }

    void CellViewer::showInstanceWidget()
    {
        instanceWidget_ = new InstanceWidget();
        instanceWidget_->setCellViewer(this);
        openCell();
        instanceWidget_->setCell(getCell());
        instanceWidget_->show();
    }

    /*void CellViewer::showNetsWidget()
    {
        netsWidget_ = new NetsWidget();
        netsWidget_->setCellViewer(this);
        netsWidget_->setCell(getCell());
        netsWidget_->show();
    }*/

    void CellViewer::setCell(Cell* cell)
    {
        cellWidget_->setCell(cell);
        iconWidget_->setCell(cell);
    }

    void CellViewer::saveCell()
    {
        Cell* cell = getCell();
        if (cell == NULL) return;
        QString cellName = cell->getName().c_str();
        if (saveCellDialog_->run(cellName))
        {
            cell->setName(cellName.toStdString());
            cell->save();
            //cell->save(cellName);
        }
    }

    void CellViewer::showIconViewer()
    {
        iconWidget_->setWindowTitle("Icon Viewer");
        iconWidget_->setMinimumSize(iconWidget_->minimumSizeHint());
        iconWidget_->show();
    }

    void CellViewer::showQuitDialog()
    {
        QDialog* quitDialog = new QDialog();
        QPushButton* okButton = new QPushButton("OK");
        QPushButton* cancelButton = new QPushButton("Cancel");
        okButton->setFont(QFont("Comic Sans MS", 10, QFont::Bold, true));
        okButton->setCursor(Qt::PointingHandCursor);
        cancelButton->setFont(QFont("Comic Sans MS", 10, QFont::Bold, true));
        cancelButton->setCursor(Qt::PointingHandCursor);
        cancelButton->setDefault(true);

        QLabel* label = new QLabel("Are you sure you want to quit?");
        QHBoxLayout* buttonLayout = new QHBoxLayout();
        buttonLayout->addStretch();
        buttonLayout->addWidget(okButton);
        buttonLayout->addStretch();
        buttonLayout->addWidget(cancelButton);
        buttonLayout->addStretch();

        QVBoxLayout* mainLayout = new QVBoxLayout();
        mainLayout->addStretch();
        mainLayout->addWidget(label);
        mainLayout->addStretch();
        mainLayout->addLayout(buttonLayout);
        mainLayout->setSizeConstraint(QLayout::SetFixedSize);

        connect(okButton, SIGNAL(clicked()), quitDialog, SLOT(accept()));
        connect(cancelButton, SIGNAL(clicked()), quitDialog, SLOT(reject()));

        quitDialog->setLayout(mainLayout);
        quitDialog->setWindowTitle("Quit Dialog");

        int result = quitDialog->exec();
        if (result == QDialog::Accepted)
        {
            iconWidget_->close();
            close();
        }
    }


} // Netlist namespace.

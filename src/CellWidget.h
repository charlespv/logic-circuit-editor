// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_H
#define NETLIST_CELL_WIDGET_H

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
class QPainter;
#include "Box.h"


namespace Netlist {

  class Cell;
  class NodeTerm;


  class CellWidget : public QWidget {
      Q_OBJECT;
    public:
                      CellWidget         ( QWidget* parent=NULL );
      virtual        ~CellWidget         ();
              void    setCell            ( Cell* );
      inline  Cell*   getCell            () const;
      inline int    xToScreenX(int x) const;
      inline  int     yToScreenY         ( int y ) const;
      inline  QRect   boxToScreenRect    ( const Box& ) const;
      inline  QPoint  pointToScreenPoint ( const Point& ) const;
      inline  int     screenXToX         ( int x ) const;
      inline  int     screenYToY         ( int y ) const;
      inline  Box     screenRectToBox    ( const QRect& ) const;
      inline  Point   screenPointToPoint ( const QPoint& ) const;
      virtual QSize   minimumSizeHint    () const;
      virtual void    resizeEvent        ( QResizeEvent* );
    protected:
      virtual void    paintEvent         ( QPaintEvent* );
      virtual void    keyPressEvent      ( QKeyEvent* );
    private:
      Cell* cell_;
      Box viewport_;
    public slots:
      void    goLeft             ();
      void    goRight            ();
      void    goUp               ();
      void    goDown             ();
  };


  inline Cell* CellWidget::getCell () const { return cell_; }


}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_H

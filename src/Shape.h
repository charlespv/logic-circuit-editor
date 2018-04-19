#ifndef NETLIST_SHAPE_H
#define NETLIST_SHAPE_H

#include "Box.h"
#include "Term.h"
#include "Line.h"

namespace Netlist
{
	class Symbol;
	
	class Shape
	{
		public:
			Shape(Symbol*);
			~Shape();
			inline Symbol* getSymbol() const;
			virtual Box getBoundingBox() const = 0;
			virtual void toXml(std::ostream&) const = 0;
			static Shape* fromXml(Symbol*, xmlTextReaderPtr);
		private:
			Symbol* owner_;
	};

	inline Symbol* Shape::getSymbol() const { return owner_; }

	class BoxShape: public Shape
	{
		public:
			BoxShape(Symbol*, const Box&);
			~BoxShape();
			virtual Box getBoundingBox() const;
			virtual void toXml(std::ostream&) const;
			static Shape* fromXml(Symbol*, xmlTextReaderPtr);
		private:
			Box box_;
	};

	class LineShape: public Shape
	{
		public:
			LineShape(Symbol*, int, int, int, int);
			~LineShape();
			inline int getX1() const;
			inline int getY1() const;
			inline int getX2() const;
			inline int getY2() const;
			virtual Box getBoundingBox() const;
			virtual void toXml(std::ostream&) const;
			static Shape* fromXml(Symbol*, xmlTextReaderPtr);
		private:
			int x1_;
			int y1_;
			int x2_;
			int y2_;
	};

	inline int LineShape::getX1() const { return x1_; }
	inline int LineShape::getY1() const { return y1_; }
	inline int LineShape::getX2() const { return x2_; }
	inline int LineShape::getY2() const { return y2_; }

	class TermShape: public Shape
	{
		public:
			enum NameAlign {TopLeft = 1, TopRight, BottomLeft, BottomRight, Misplaced};
		public:
			TermShape(Symbol*, const std::string&, int, int, NameAlign);
			~TermShape();
			inline Term* getTerm() const;
			inline int getX() const;
			inline int getY() const;
			inline NameAlign getAlign() const;
			virtual Box getBoundingBox() const;
			virtual void toXml(std::ostream&) const;
			static Shape* fromXml(Symbol*, xmlTextReaderPtr);
			static std::string toString(NameAlign);
			static NameAlign toNameAlign(std::string);
		private:
			Term* term_;
			int x_;
			int y_;
			NameAlign align_;
	};

	inline Term* TermShape::getTerm() const { return term_; }
	inline int 	TermShape::getX() const { return x_; }
	inline int TermShape::getY() const { return y_; }
	inline TermShape::NameAlign TermShape::getAlign() const { return align_; }

	class EllipseShape: public Shape
	{
		public:
			EllipseShape(Symbol*, const Box&);
			~EllipseShape();
			virtual Box getBoundingBox() const;
			virtual void toXml(std::ostream&) const;
			static Shape* fromXml(Symbol*, xmlTextReaderPtr);
		private:
			Box box_;
	};

	class ArcShape: public Shape
	{
		public:
			ArcShape(Symbol*, const Box&, int, int);
			~ArcShape();
			inline int getStart() const;
			inline int getSpan() const;
			virtual Box getBoundingBox() const;
			virtual void toXml(std::ostream&) const;
			static Shape* fromXml(Symbol*, xmlTextReaderPtr);
		private:
			Box box_;
			int start_;
			int span_;
	};

	inline int ArcShape::getStart() const { return start_; }
	inline int ArcShape::getSpan() const { return span_; }
}	// Netlist namespace.

#endif // NETLIST_SHAPE_H


#include "Shape.h"
#include "Symbol.h"
#include "XmlUtil.h"
#include "Cell.h"

namespace Netlist
{
	using namespace std;

	Shape::Shape(Symbol* symbol)
		: owner_(symbol)
	{
		owner_->add(this);
	}

	Shape::~Shape() 
	{
		owner_->remove(this);
	}

	Shape* Shape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		const xmlChar* boxTag = xmlTextReaderConstString(reader, (const xmlChar*)"box");
		const xmlChar* ellipseTag = xmlTextReaderConstString(reader, (const xmlChar*)"ellipse");
		const xmlChar* arcTag = xmlTextReaderConstString(reader, (const xmlChar*)"arc");
		const xmlChar* lineTag = xmlTextReaderConstString(reader, (const xmlChar*)"line");
		const xmlChar* termTag = xmlTextReaderConstString(reader, (const xmlChar*)"term");
		const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
		Shape* shape = NULL;
		
		if (nodeName == boxTag)
			shape = BoxShape::fromXml(owner, reader);
		if (nodeName == ellipseTag)
			shape = EllipseShape::fromXml(owner, reader);
		if (nodeName == arcTag)
			shape = ArcShape::fromXml(owner, reader);
		if (nodeName == lineTag)
			shape = LineShape::fromXml(owner, reader);
		if (nodeName == termTag)
			shape = TermShape::fromXml(owner, reader);

		if (shape == NULL)
			cerr << "[ERROR] Unknown of misplaced tag <" << nodeName << "> (line: "
				   << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return shape;
	}

	BoxShape::BoxShape(Symbol* symbol, const Box& box)
		: Shape(symbol)
		, box_(box)
	{}

	BoxShape::~BoxShape()
	{}
	
	Box BoxShape::getBoundingBox() const
	{
		return box_;
	}

	void BoxShape::toXml(ostream& o) const
	{
		o << indent << "<box x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1()
			<< "\" x2=\"" << box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
	}

	Shape* BoxShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
		{
			int x1, y1, x2, y2;
			if ((xmlGetIntAttribute(reader, "x1", x1)) && (xmlGetIntAttribute(reader, "y1", y1)) &&
					(xmlGetIntAttribute(reader, "x2", x2)) && (xmlGetIntAttribute(reader, "y2", y2)))
			{
				return (Shape*)(new BoxShape(owner, Box(x1, y1, x2, y2)));		
			}
		}
		cerr << "[ERROR]: BoxShape::fromXml(): Unknown 'box' format (line:" 
				 << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return NULL;
	}

	LineShape::LineShape(Symbol* symbol, int x1, int y1, int x2, int y2)
		: Shape(symbol)
		, x1_(x1)
		, y1_(y1)
		, x2_(x2)
		, y2_(y2)
	{}

	LineShape::~LineShape()
	{}

	Box LineShape::getBoundingBox() const
	{
		return Box(x1_, y1_, x2_, y2_);
	}

	void LineShape::toXml(ostream& o) const
	{
		o << indent << "<line x1=\"" << x1_ << "\" y1=\"" << y1_
			<< "\" x2=\"" << x2_ << "\" y2=\"" << y2_ << "\"/>\n";
	}
	
	Shape* LineShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{ 
		if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
		{
			int x1, y1, x2, y2;
			if ((xmlGetIntAttribute(reader, "x1", x1)) && (xmlGetIntAttribute(reader, "y1", y1)) &&
					(xmlGetIntAttribute(reader, "x2", x2)) && (xmlGetIntAttribute(reader, "y2", y2)))
			{
				return (Shape*)(new LineShape(owner, x1, y1, x2, y2));
			}
		}
		cerr << "[ERROR]: LineShape::fromXml(): Unknown 'line' format (line:"
				 << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return NULL;
	}

	TermShape::TermShape(Symbol* symbol, const string& name, int x, int y, TermShape::NameAlign align)
		: Shape(symbol)
		, term_(NULL)
		, x_(x)
		, y_(y)
		, align_(align)
	{
		term_ = getSymbol()->getCell()->getTerm(name);
	}

	TermShape::~TermShape()
	{}

	Box TermShape::getBoundingBox() const
	{
		return Box(x_, y_, x_, y_);
	}

	string TermShape::toString(TermShape::NameAlign align)
	{
		if (align == TopLeft) return "top_left";
		if (align == TopRight) return "top_right";
		if (align == BottomLeft) return "bottom_left";
		if (align == BottomRight)	return "bottom_right";
		cerr << "[ERROR]: TermShape::toString(): TermShape::NameAlign invalid." << endl;
		return string();
	}
	
	TermShape::NameAlign TermShape::toNameAlign(string align)
	{
		if (align == "top_left") return TopLeft;
		if (align == "top_right") return TopRight;
		if (align == "bottom_left") return BottomLeft;
		if (align == "bottom_right") return BottomRight;
		cerr << "[ERROR]: TermShape::toNameAlign(): Align invalid." << endl;
		return Misplaced;
	}
	
	void TermShape::toXml(ostream& o) const
	{
		o << indent << "<term name=\"" << term_->getName() << "\" x1=\"" << x_ 
			<< "\" y1=\"" << y_ << "\" align=\"" << toString(getAlign()) << "\"/>\n";
	}

	Shape* TermShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
		{
			string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
			if (not termName.empty())
			{
				int x, y;
				if ((xmlGetIntAttribute(reader, "x1", x)) && (xmlGetIntAttribute(reader, "y1", y)))
				{
					string align = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"align"));
					if (not align.empty())
					{
						return (Shape*)(new TermShape(owner, termName, x, y, toNameAlign(align)));
					}
					cerr << "[ERROR] TermShape::fromXml(): \"align\" attribute missing (line:"
							 << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
					return NULL;
				}
				cerr << "[ERROR] TermShape::fromXml(): \"x\" or \"y\" attribute missing (line:"
						 << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
				return NULL;
			}
			cerr << "[ERROR] TermShape::fromXml(): \"name\" attribute missing (line:" 
					 << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
			return NULL;
		}
		cerr << "[ERROR] TermShape::fromXml(): Unknown 'term' format (line:"
				 << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return NULL;
	}
	
	EllipseShape::EllipseShape(Symbol* owner, const Box& box)
		: Shape(owner)
		, box_(box)
	{}

	EllipseShape::~EllipseShape()
	{}

	Box EllipseShape::getBoundingBox() const
	{
		return box_;
	}

	void EllipseShape::toXml(ostream& o) const
	{
		o << indent << "<ellipse x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" 
			<< box_.getX2() << "\" y2=\"" << box_.getY2() << "\"/>\n";
	}

	Shape* EllipseShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
		{
			int x1, y1, x2, y2;
			if ((xmlGetIntAttribute(reader, "x1", x1)) && (xmlGetIntAttribute(reader, "y1", y1)) &&
					(xmlGetIntAttribute(reader, "x2", x2)) && (xmlGetIntAttribute(reader, "y2", y2)))
			{
				return (Shape*)(new EllipseShape(owner, Box(x1, y1, x2, y2)));		
			}
		}
		cerr << "[ERROR]: EllipseShape::fromXml(): Unknown 'ellipse' format (line:" 
				 << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return NULL;
	}

	ArcShape::ArcShape(Symbol* owner, const Box& box, int start, int span)
		: Shape(owner)
		, box_(box)
		, start_(start)
		, span_(span)
	{}

	ArcShape::~ArcShape()
	{}

	Box ArcShape::getBoundingBox() const
	{
		return box_;
	}

	void ArcShape::toXml(ostream& o) const
	{
		o << indent << "<arc x1=\"" << box_.getX1() << "\" y1=\"" << box_.getY1() << "\" x2=\"" << box_.getX2() 
			<< "\" y2=\"" << box_.getY2() << "\" start=\"" << getStart() << "\" span=\"" << getSpan() << "\"/>\n";
	}

	Shape* ArcShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT)
		{
			int x1, y1, x2, y2, start, span;
			if ((xmlGetIntAttribute(reader, "x1", x1)) && (xmlGetIntAttribute(reader, "y1", y1)) &&
					(xmlGetIntAttribute(reader, "x2", x2)) && (xmlGetIntAttribute(reader, "y2", y2)) &&
					(xmlGetIntAttribute(reader, "start", start)) && (xmlGetIntAttribute(reader, "span", span)))
			{
				return (Shape*)(new ArcShape(owner, Box(x1, y1, x2, y2), start, span));
			}
		}
		cerr << "[ERROR]: ArcShape::fromXml(): Unknown 'arc' format (line:"
			   << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return NULL;
	}
} // Netlist namespace.


	
		


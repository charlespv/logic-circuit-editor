#include "Symbol.h"
#include "Cell.h"
#include "Term.h"
#include "Shape.h"

namespace Netlist
{
	using namespace std;
	
	Symbol::Symbol(Cell* owner)
		: owner_(owner)
		, shapes_()
	{}

	Symbol::~Symbol()
	{
		while (not shapes_.empty()) delete *(shapes_.begin());
	}

	Box Symbol::getBoundingBox() const
	{
		Box b;
		for (size_t i = 0; i < shapes_.size(); ++i)
			b.merge(shapes_[i]->getBoundingBox());
		return b;
	}

	Point Symbol::getTermPosition(string& name) const
	{
		for (size_t i = 0; i < shapes_.size(); ++i)
		{
			TermShape* termShapeTmp = dynamic_cast<TermShape*>(shapes_[i]);
			if (termShapeTmp)
			{
				if (termShapeTmp->getTerm()->getName() == name)
					return Point(termShapeTmp->getX(), termShapeTmp->getY());
			}
		}
		cerr << "[ERROR] Symbol::getTermPosition(): Term \"" << name << "\" not found.\n";
		return Point();
	}

	TermShape* Symbol::getTermShape(string& name) const
	{
		TermShape* termShapeTmp = NULL;
		for (size_t i = 0; i < shapes_.size(); ++i)
		{
			termShapeTmp = dynamic_cast<TermShape*>(shapes_[i]);
			if (termShapeTmp)
			{
				if (termShapeTmp->getTerm()->getName() == name)
					break;
			}
		}
		return termShapeTmp;
	}

	void Symbol::add(Shape* shape)
	{
			shapes_.push_back(shape);
	}

	void Symbol::remove(Shape* shape)
	{
		for (vector<Shape*>::iterator ishape = shapes_.begin(); ishape != shapes_.end(); ++ishape)
		{
			if (*ishape == shape)
				shapes_.erase(ishape);
		}
	}
	
	void Symbol::toXml(ostream& o) const
	{
		for (size_t i = 0; i < shapes_.size(); ++i)
		{
			shapes_[i]->toXml(o);
		}
	}
	
	Symbol* Symbol::fromXml(Cell* cell, xmlTextReaderPtr reader)
	{
		const xmlChar* symbolTag = xmlTextReaderConstString(reader, (const xmlChar*)"symbol");
		Symbol* symbol = cell->getSymbol();
		while (true)
		{
			int status = xmlTextReaderRead(reader);
			if (status != 1)
			{
				if (status)
				{
					cerr << "[ERROR] Net::fromXml(): Unexpected termination of the Xml parser (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
					return NULL;
				}
			}

			switch (xmlTextReaderNodeType(reader))
			{
				case XML_READER_TYPE_COMMENT:
				case XML_READER_TYPE_WHITESPACE:
				case XML_READER_TYPE_SIGNIFICANT_WHITESPACE:
					continue;
			}
			
			const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
			
			if ((nodeName == symbolTag) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT))
			{
				return symbol;
			}
			if ((nodeName != symbolTag) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT))
			{
				if (Shape::fromXml(symbol, reader)) 
				{
					continue;
				}
				break;
			}
			else
				break;
		}
		cerr << "[ERROR]: Symbol::fromXml(): Problem in analizing \"shape\" tag (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return NULL;
	}
} // Netlist namespace.


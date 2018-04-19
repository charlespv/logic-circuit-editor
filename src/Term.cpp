#include "Term.h"
#include "Instance.h"
#include "Cell.h"
#include "Net.h"
#include "XmlUtil.h"

namespace Netlist
{
	using namespace std;
	
	Term::Term(Cell* cell, const string& name, Term::Direction direction)
		: owner_(cell)
		, name_(name)
		, direction_(direction)
		, type_(External)
		, net_(NULL)
		, node_(this)
	{
		cell->add(this);
	}

	Term::Term(Instance* instance, const string& name, Term::Direction direction)
		: owner_(instance)
		, name_(name)
		, direction_(direction)
		, type_(Internal)
		, net_(NULL)
		, node_(this)
	{
		instance->add(this);
	}

	Term::~Term()
	{
		if (isInternal()) static_cast<Instance*>(owner_)->remove(this);
		static_cast<Cell*>(owner_)->remove(this);
		if (getNet()) getNet()->remove(&node_);
	}

	string Term::toString(Term::Type type)
	{
		if (type == Internal) return "Internal";
		if (type == External) return "External";
		cerr << "[ERROR] Term::toString(Term::Type): Term::Type invalid." << endl;
		return string();
	}

	string Term::toString(Term::Direction direction)
	{
		if (direction == In) return "In";
		if (direction == Out) return "Out";
		if (direction == Inout) return "Inout";
		if (direction == Tristate) return "Tristate";
		if (direction == Transcv) return "Transcv";
		if (direction == Unknown) return "Unknown";
		cerr << "[ERROR] Term::toString(Term::Direction): Term::Direction invalid." << endl;
		return string();
	}

	Term::Direction Term::toDirection(string direction)
	{
		if (direction == "In") return In;
		if (direction == "Out") return Out;
		if (direction == "Inout") return Inout;
		if (direction == "Tristate") return Tristate;
		if (direction == "Transcv") return Transcv;
		return Unknown;
	}
	
	Term::Type Term::toType(string type)
	{
		if (type == "Internal") return Internal;
		if (type == "External") return External;
		cerr << "[ERROR]: Term::toType(): Type invalid." << endl;
		return Invalid;
	}

	Cell* Term::getOwnerCell() const
	{
		if (isExternal()) return (Cell*)owner_; 
		return static_cast<Instance*>(owner_)->getCell();
	}

	void Term::setNet(Net* net)
	{
		net_ = net;
		net_->add(&node_);
	}

	void Term::setNet(const string& netName)
	{
		Net* netTmp = getOwnerCell()->getNet(netName);
		if (netTmp == NULL)
		{
			Net* newNet = new Net(getOwnerCell(), netName, getType());
			setNet(newNet);
		} 
		else 
		{
			setNet(netTmp);
		}
	}

	void Term::setPosition(const Point& pos)
	{
		node_.setPosition(pos);
	}

	void Term::setPosition(int x, int y)
	{
		node_.setPosition(x, y);
	}

	void Term::toXml(ostream& o) const
	{
		if (toString(getDirection()).empty())
		{
			cerr << "[ERROR]: Term::toXml(): Term::Direction invalid." << endl;
			return;
		}
		o << indent << "<term name=\"" << getName() << "\" direction=\"" << toString(getDirection()) 
			<< "\" x=\"" << getPosition().getX() << "\" y=\"" << getPosition().getY() << "\"/>\n";
	}

	Term* Term::fromXml(Cell* cell, xmlTextReaderPtr reader)
	{
		const xmlChar* termTag = xmlTextReaderConstString(reader, (const xmlChar*)"term");
		const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
		Term* term = NULL;
		if ((nodeName == termTag) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT))
		{
			string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
			if (not termName.empty())
			{
				string termDirection = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"direction"));
				term = new Term(cell, termName, toDirection(termDirection));
				int x = 0, y = 0;
				xmlGetIntAttribute(reader, "x", x);
				xmlGetIntAttribute(reader, "y", y); 
				term->setPosition(x, y);
				return term;
			}
			cerr << "[ERROR] Term::fromXml(): \"name\" attribute missing (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
			return term;
		}
		cerr << "[ERROR] Term::fromXml(): Unknown or misplaced tag <" << nodeName
				 << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return term;
	}
} // Netlist namespace

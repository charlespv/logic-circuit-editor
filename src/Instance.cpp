#include "Instance.h"
#include "Cell.h"
#include "Term.h"
#include "XmlUtil.h"
#include <cstdlib>

namespace Netlist
{
	using namespace std;

	Instance::Instance(Cell* owner, Cell* model, const string& name)
		: owner_(owner)
		, masterCell_(model)
		, name_(name)
		, terms_()
		, position_()
	{
		owner_->add(this);
		vector<Term*> terms = model->getTerms();
		for (size_t i = 0; i < terms.size(); i++)
		{
			new Term(this, terms[i]->getName(), terms[i]->getDirection()); // L'ajout du nouveau 'Term' dans le vector<Term*> 
		}																																 // est fait dans le constructeur de Term.
	}

	Instance::~Instance()
	{
		owner_->remove(this);
		while (not terms_.empty()) delete *(terms_.begin());
	}

	Term* Instance::getTerm(const string& name) const
	{
		for (vector<Term*>::const_iterator iterm = terms_.begin(); iterm != terms_.end(); ++iterm)
		{
			if ((*iterm)->getName() == name) return *iterm;
		}
		return NULL;
	}

	bool Instance::connect(const string& name, Net* net)
	{
		Term* term = getTerm(name);
		if (term == NULL) return false;
		term->setNet(net);
		return true;
	}

	void Instance::add(Term* term)
	{
		if (getTerm(term->getName()))
		{
			cerr << "[ERROR] Attempt to add duplicated terminal <" << term->getName() << ">." << endl;
			exit(1);
		}
		terms_.push_back(term);
	}

	void Instance::remove(Term* term)
	{
		for (vector<Term*>::iterator iterm = terms_.begin(); iterm != terms_.end(); ++iterm)
		{
			if (*iterm == term) terms_.erase(iterm);
		}
	}

	void Instance::setPosition(const Point& pos)
	{
		position_ = pos;
		for (size_t i = 0; i < terms_.size(); ++i)
		{
			string name = terms_[i]->getName();
			Point coorTermShape = getMasterCell()->getSymbol()->getTermPosition(name);
			terms_[i]->setPosition(pos.getX() + coorTermShape.getX(), pos.getY() + coorTermShape.getY());
		}
	}
	
	void Instance::setPosition(int x, int y)
	{
		position_ = Point(x, y);
		for (size_t i = 0; i < terms_.size(); ++i)
		{
			string name = terms_[i]->getName();
			Point coorTermShape = getMasterCell()->getSymbol()->getTermPosition(name);
			terms_[i]->setPosition(x + coorTermShape.getX(), y + coorTermShape.getY());
		}
	}

	void Instance::toXml(ostream& o) const
	{
		o << indent << "<instance name=\"" << getName() << "\" mastercell=\"" << masterCell_->getName() 
			<< "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
	}

	Instance* Instance::fromXml(Cell* cell, xmlTextReaderPtr reader)
	{
		const xmlChar* instanceTag = xmlTextReaderConstString(reader, (const xmlChar*)"instance");
		const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
		Instance* instance = NULL;

		if ((nodeName == instanceTag) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT))
		{
			string instanceName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
			if (not instanceName.empty())
			{
				string masterCellName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"mastercell"));
				Cell* masterCell = Cell::find(masterCellName);
				if (masterCell)
				{
					instance = new Instance(cell, masterCell, instanceName);
					int x = 0, y = 0;
					int& rx = x;
					int& ry = y;
					if (xmlGetIntAttribute(reader, "x", rx) && xmlGetIntAttribute(reader, "y", ry)) instance->setPosition(x, y);
					return instance;
				}
				cerr << "[ERROR] Instance::fromXml(): \"mastercell\" attribute missing (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
				return instance;
			}
			cerr << "[ERROR] Instance::fromXml(): \"name\" attribute missing (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
			return instance;
		}
		cerr << "[ERROR] Instance::fromXml(): Unknown or misplaced tag <" << nodeName 
				 << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return instance;
	}
} // Netlist namespace

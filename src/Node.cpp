#include "Node.h"
#include "Term.h"
#include "Cell.h"
#include "Net.h"
#include "Line.h"
#include "XmlUtil.h"
#include "Instance.h"
#include <limits>

namespace Netlist
{
	using namespace std;
	
	const size_t Node::noid = numeric_limits<size_t>::max();

	Node::Node(size_t id)
		: id_(id)
		, lines_()
		, position_()
	{}

	Node::~Node()
	{
		while (not lines_.empty()) delete lines_[0];
	}

	void Node::attach(Line* line)
	{
		for (size_t i = 0; i < lines_.size(); ++i)
		{
			if (lines_[i] == line) return;
		}
		lines_.push_back(line);
	}
	
	void Node::detach(Line* line)
	{
		vector<Line*>::iterator iline = lines_.begin();
		for (; iline != lines_.end(); ++iline)
		{
			if ((*iline) == line)
			{
				lines_.erase(iline);
				break;
			}
		}
	}

	NodePoint::NodePoint(Net* net, size_t id, Point p)
		: Node(id)
		, net_(net)
	{	
		setPosition(p);
		net_->add(this);
	}

	NodePoint::~NodePoint()
	{
		net_->remove(this);
	}

	Net* NodePoint::getNet() const
	{
		return net_;
	}

	void NodePoint::toXml(ostream& o) const
	{
		o << indent << "<node x=\"" << position_.getX() << "\" y=\""
			<< position_.getY() << "\" id=\"" << id_ << "\"/>\n";
	}

	NodeTerm::NodeTerm(Term* term, size_t id)
		: Node(id)
		, term_(term)
	{}
	
	NodeTerm::~NodeTerm()
	{
		if (getNet()) 
			getNet()->remove(this);
	}

	Net* NodeTerm::getNet() const
	{
		return (this == NULL) ? NULL : term_->getNet();
	}

	void NodeTerm::toXml(ostream& o) const
	{
		if (this == NULL) return;
		if (term_->isExternal())
			o << indent << "<node term=\"" << term_->getName() << "\" id=\"" << id_ 
				<< "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
		else
			o << indent << "<node term=\"" << term_->getName() << "\" instance=\"" << term_->getInstance()->getName() 
				<< "\" id=\"" << id_ << "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
	}

	bool Node::fromXml(Net* net, xmlTextReaderPtr reader)
	{
		const xmlChar* nodeTag = xmlTextReaderConstString(reader, (const xmlChar*)"node");
		const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
		int id;

		if ((nodeName == nodeTag) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT))
		{
			string instanceName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"instance"));
			string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"term"));
			if (not xmlGetIntAttribute(reader, "id", id)) 
				return false;
			
			if (termName.empty())
			{
				int x = 0, y = 0;
				if ((xmlGetIntAttribute(reader, "x", x)) && (xmlGetIntAttribute(reader, "y", y)))
				{
					new NodePoint(net, (size_t)id, Point(x, y));
					return true;
				}
				return false;
			}

			Term* term = NULL;
			if (instanceName.empty())
			{
				term = net->getCell()->getTerm(termName);
				if (not term)
				{
					cerr << "[ERROR] Node::fromXml(): No Term <" << termName << "> in Instance <" << instanceName 
						   << "> (line: " << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
					return false;
				}
			}
			else
			{
				Instance* instance = net->getCell()->getInstance(instanceName);
				if (not instance)
				{
					cerr << "[ERROR] Node::fromXml(): No Instance <" << instanceName << "> in Cell <" << net->getCell()->getName() 
						   << "> (line: " << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
					return false;
				}
				term = instance->getTerm(termName);
				if (not term)
				{
					cerr << "[ERROR] Node::fromXml(): No Term <" << termName << "> in Instance <" << instanceName 
							 << "> (line: " << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
					return false;
				}
			}
			term->getNode()->setId(id);
			term->setNet(net);		
			return true;
		}
		
		cerr << "[ERROR] Node::fromXml(): Unknown or misplaced tag <" << nodeName
				 << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return false;
	}
} // Netlist namespace
	
		

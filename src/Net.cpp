#include "Net.h"
#include "Node.h"
#include "Cell.h"
#include "XmlUtil.h"
#include "Line.h"
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>

namespace Netlist
{
	using namespace std;
	
	Net::Net(Cell* cell, const string& name, Term::Type type)
		: owner_(cell)
		, name_(name)
		, id_(cell->newNetId())
		, type_(type)
		, nodes_()
	{
		cell->add(this);
	}

	Net::~Net()
	{
		if (getCell()) owner_->remove(this);
		while (not nodes_.empty())  nodes_.erase(nodes_.begin());
	}

	size_t Net::getFreeNodeId() const
	{
		for (size_t i = 0; i < nodes_.size() ; ++i)
		{
			if (nodes_[i] == NULL)	return i;
		}
		return Node::noid; 
	}
	
	Node* Net::getNode(size_t id) const
	{
		for (size_t i = 0; i < nodes_.size(); ++i)
		{
			if (nodes_[i]->getId() == id)
				return nodes_[i];
		}
		return NULL;
	}

	void Net::add(Node* node)
	{
		if (not node)
			return;
		size_t id = node->getId();
		size_t nodesSize = nodes_.size();
		
		if (id != Node::noid)															// On teste si il le noeud à ajouter possède déjà 1 id valide.
		{
			for (vector<Node*>::iterator inode = nodes_.begin(); inode != nodes_.end(); ++inode) // Test pour voir si l'id existe déjà.
			{
				if (*inode == NULL) continue;
				if ((*inode)->getId() == id)
				{
					cerr << "[ERROR] Attempt to add duplicated node <" << id << ">." << endl;
					exit(1);
				}
			}

			if (id < nodesSize)														  // Si l'id du noeud à ajouter est inférieur au nombre d'éléments ...
			{
				nodes_[id] = node;                            // ... on peut l'ajouter à l'aide de l'opérateur [].
				return;
			}
			else								 														// Si l'id du noeud à ajouter dépasse le nombre d'éléments... 
			{
				for (size_t j = nodesSize; j < id; ++j)       // ... il faut donc ajouter (id - 1 - nombre d'éméments) cases NULL...
				{
					nodes_.push_back(NULL);
				}
				nodes_.push_back(node);												// ... avant d'ajouter le noeud numéro i dans le tableau.	
				return;                                       // On a pu absorber le cas (i = nodesSize) 
			}
		} 
								
		if (nodesSize != 0)															  // On traite le cas où il faut ajouter et attribuer 1 id au noeud.
		{
			id = getFreeNodeId();													  // On cherche dans le tableau si il reste une place libre à insérer
			if (id != Node::noid)
			{
				nodes_[id] = node;
				node->setId(id);
				return;
			}
		}
		nodes_.push_back(node);	// On ajoute a la fin du vector<Node*> si le tableau est vide ou si il n'y a plus de place libre (i == Node::noid).
		node->setId(nodes_.size() - 1);
	}
	
	void Net::add(Line* line)
	{
		if (line)
			lines_.push_back(line);
	}

	bool Net::remove(Node* node)
	{
		for (vector<Node*>::iterator inode = nodes_.begin(); inode != nodes_.end(); ++inode)
		{
			if (*inode == node) 
			{
				delete node;	  // Il s'agit ici non pas d'un 'remove' comme on a fait avec les autres vecteurs (erase) 
				*inode == NULL; // mais de retirer l'element souhaite du tableau et le remplace par le pointeur NULL.
				return true;
			}
		}
		return false;
	}
	
	bool Net::remove(Line* line)
	{
		if (line)
			for (vector<Line*>::iterator iline = lines_.begin(); iline != lines_.end(); ++iline)
				if (*iline == line)
				{
					lines_.erase(iline);
					return true;
				}
		return false;
	}

	void Net::toXml(ostream& o) const
	{
		if (Term::toString(getType()).empty())
		{
			cerr << "[ERROR]: Net::toXml(): Term::Type invalid." << endl;
			return;
		}
		o << indent++ << "<net name=\"" << getName() << "\" type=\"" << Term::toString(getType()) << "\"/>\n";
		for (vector<Node*>::const_iterator inode = nodes_.begin(); inode != nodes_.end(); ++inode)
		{
			if (*inode != NULL)
				(*inode)->toXml(o);
		}
		for (vector<Line*>::const_iterator iline = lines_.begin(); iline != lines_.end(); ++iline)
		{
			if (*iline != NULL)
				(*iline)->toXml(o);
		}
		o << --indent << "</net>\n";
	}

	Net* Net::fromXml(Cell* cell, xmlTextReaderPtr reader)
	{
		const xmlChar* netTag = xmlTextReaderConstString(reader, (const xmlChar*)"net");
		const xmlChar* lineTag = xmlTextReaderConstString(reader, (const xmlChar*)"line");
		const xmlChar* nodeName = xmlTextReaderConstLocalName(reader);
		Net* net = NULL;

		if ((netTag == nodeName) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT))
		{
			string netName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"name"));
			if (not netName.empty())
			{
				Term::Type netType = Term::toType(xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*)"type")));
				net = new Net(cell, netName, netType);

				while (true)
				{
					int status = xmlTextReaderRead(reader);
					if (status != 1)
					{
						if (status)
						{
							cerr << "[ERROR] Net::fromXml(): Unexpected termination of the Xml parser (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
							delete net;
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
					
					nodeName = xmlTextReaderConstLocalName(reader);
					
					if ((nodeName == netTag) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT))
					{
						return net;
					}
					if ((nodeName != lineTag) && (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT))
					{
						if (Node::fromXml(net, reader)) 
						{
							continue;
						}
						break;
					}
					else
					{
						if (Line::fromXml(net, reader)) 
						{
							continue;
						}
						break;
					}
				}
			}
			else
			{
				cerr << "[ERROR] Net::fromXml(): \"name\" attribute missing (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
				return net;
			}
		}
		cerr << "[ERROR] Net::fromXml(): Unknown or misplaced tag <" << nodeName
				 << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << endl;
		return net;
	}
} // Netlist namespace

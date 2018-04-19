#ifndef NETLIST_NET_H
#define NETLIST_NET_H

#include <string>
#include <vector>
#include "Term.h"
#include <libxml/xmlreader.h>

namespace Netlist
{
	class Cell;
	class Node;
	class Line;

	class Net
	{
		public:
			static Net* fromXml(Cell*, xmlTextReaderPtr);
		public: 
			// CTOR
			Net(Cell*, const std::string&, Term::Type);
			// DTOR
			~Net();
			// Accessors
			inline Cell* getCell() const;
			inline const std::string& getName() const;
			inline unsigned int getId() const;
			inline Term::Type getType() const;
			inline const std::vector<Node*>& getNodes() const;
			inline const std::vector<Line*>& getLines() const;
			Node* getNode(size_t) const;
			size_t getFreeNodeId() const;
			// Mutators
			void add(Node*);
			void add(Line*);
			bool remove(Node*);
			bool remove(Line*);
			void toXml(std::ostream&) const;
		private:
			Cell* owner_;
			std::string name_;
			unsigned int id_;
			Term::Type type_;
			std::vector<Node*> nodes_;
			std::vector<Line*> lines_;
	};

	inline Cell* Net::getCell() const
	{
		return owner_;
	}

	inline const std::string& Net::getName() const
	{
		return name_;
	}

	inline unsigned int Net::getId() const
	{
		return id_;
	}

	inline Term::Type Net::getType() const
	{
		return type_;
	}
	inline const std::vector<Node*>& Net::getNodes() const
	{
		return nodes_;
	}
	inline const std::vector<Line*>& Net::getLines() const
	{
		return lines_;
	}
} // Netlist namespace

#endif // NETLIST_NET_H


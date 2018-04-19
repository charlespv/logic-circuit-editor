#ifndef NETLIST_CELL_H
#define NETLIST_CELL_H

#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "Indentation.h"
#include "Symbol.h"

namespace Netlist 
{
	class Instance;
	class Net;
	class Term;

	class Cell
	{
		public:
			static std::vector<Cell*>& getAllCells();
			static Cell* find(const std::string&);
			static Cell* fromXml(xmlTextReaderPtr);
			static Cell* load(const std::string&);
		public:
			// CTOR
			Cell(const std::string&);
			// DTOR
			~Cell();
			// Accessors
			inline const std::string& getName() const;
			inline const std::vector<Instance*>& getInstances() const;
			inline const std::vector<Term*>& getTerms() const;
			inline const std::vector<Net*>& getNets() const;
			inline Symbol* getSymbol() const;
			Instance* getInstance(const std::string&) const;
			Term* getTerm(const std::string&) const;
			Net* getNet(const std::string&) const;
			// Mutators
			void setName(const std::string&);
			void add(Instance*);
			void add(Term*);
			void add(Net*);
			void remove(Instance*);
			void remove(Term*);
			void remove(Net*);
			bool connect(const std::string&, Net* net);
			unsigned int newNetId();
			void toXml(std::ostream&) const;
			void save() const;
		private:
			static std::vector<Cell*> cells_;
			Symbol symbol_;
			std::string name_;
			std::vector<Term*> terms_;
			std::vector<Instance*> instances_;
			std::vector<Net*> nets_;
			unsigned int maxNetIds_;
	};
	
	inline const std::string& Cell::getName() const
	{
		return name_;
	}
	inline const std::vector<Instance*>& Cell::getInstances() const
	{
		return instances_;
	}
	inline const std::vector<Term*>& Cell::getTerms() const
	{
		return terms_;
	}
	inline const std::vector<Net*>& Cell::getNets() const
	{
		return nets_;
	}
	inline Symbol* Cell::getSymbol() const
	{
		return const_cast<Symbol*>(&symbol_);
	}
} // Netlist namespace

#endif // NETLIST_CELL_H

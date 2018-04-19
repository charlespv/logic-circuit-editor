#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H

#include <string>
#include "Indentation.h"
#include "Node.h"
#include "Point.h"
#include <libxml/xmlreader.h>

namespace Netlist
{
	class Net;
	class Cell;
	class Instance;
	
	class Term
	{
		public:
			enum Type {Internal = 1, External = 2, Invalid = 3};
			enum Direction {In = 1, Out = 2, Inout = 3, Tristate = 4, Transcv = 5, Unknown = 6};
		public:
			static std::string toString(Type);
			static std::string toString(Direction);
			static Direction toDirection(std::string);
			static Type toType(std::string);
			static Term* fromXml(Cell*, xmlTextReaderPtr);
		public:
			// CTOR
			Term(Cell*, const std::string& name, Direction);
			Term(Instance*, const std::string& name, Direction);
			// DTOR
			~Term();
			// Accessors
			inline bool isInternal() const;
			inline bool isExternal() const;
			inline const std::string& getName() const;
			inline Net* getNet() const;
			inline Cell* getCell() const;
			Cell* getOwnerCell() const;
			inline Instance* getInstance() const;
			inline Direction getDirection() const;
			inline Point getPosition() const;
			inline Type getType() const;
			inline NodeTerm* getNode();
			// Mutators
			void setNet(Net*);
			void setNet(const std::string&);
			inline void setDirection(Direction);
			void setPosition(const Point&);
			void setPosition(int, int);
			void toXml(std::ostream&) const;
		private:
			void* owner_;
			std::string name_;
			Direction direction_;
			Type type_;
			Net* net_;
			NodeTerm node_;
	};

	inline bool Term::isInternal() const
	{
		return (type_ == 1);
	}

	inline bool Term::isExternal() const
	{
		return (type_ == 2);
	}

	inline const std::string& Term::getName() const
	{
		return name_;
	}

	inline Net* Term::getNet() const
	{
		return net_;
	}

	inline Cell* Term::getCell() const
	{
		if (isExternal()) return (Cell*)owner_;
		return NULL;
	}

	inline Instance* Term::getInstance() const
	{
		if (isInternal()) return (Instance*)owner_;
		return NULL;
	}

	inline Term::Direction Term::getDirection() const
	{
		return direction_;
	}

	inline Point Term::getPosition() const
	{
		return node_.getPosition();
	}

	inline Term::Type Term::getType() const
	{
		return type_;
	}

	inline NodeTerm* Term::getNode() 
	{
		return &node_;
	}

	inline void Term::setDirection(Term::Direction direction)
	{
		direction_ = direction;
	}
} // Netlist namespace

#endif // NETLIST_TERM_H

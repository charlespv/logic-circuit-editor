#ifndef NETLIST_NODE_H
#define NETLIST_NODE_H

#include <iostream>
#include <vector>
#include "Indentation.h"
#include "Point.h"
#include <libxml/xmlreader.h>

namespace Netlist
{
	class Term;
	class Net;
	class Line;

	class Node
	{
		public:
			static const size_t noid;
		public:
			static bool fromXml(Net*, xmlTextReaderPtr);
		public:
			// CTOR
			Node(size_t id);
			// DTOR
			virtual ~Node();
			// Accessor
			inline Point getPosition() const;
			inline const std::vector<Line*>& getLines() const;
			inline size_t getDegree() const;
			inline size_t getId() const;
			virtual Net* getNet() const = 0;
			// Mutator
			inline void setPosition(const Point&);
			inline void setPosition(int, int);
			inline void setId(size_t);
			void attach(Line*);
			void detach(Line*);
			virtual void toXml(std::ostream&) const = 0;
		private:
			Node(const Node&);
			Node& operator=(const Node&);
		protected:
			size_t id_;
			std::vector<Line*> lines_;
			Point position_;
	};

	inline Point Node::getPosition() const
	{
		return position_;
	}
	inline const std::vector<Line*>& Node::getLines() const
	{
		return lines_;
	}
	inline size_t Node::getId() const
	{
		if (not this)
			return noid;
		return id_;
	}
	inline size_t Node::getDegree() const
	{
		return lines_.size();
	}
	inline void Node::setPosition(const Point& pos)
	{
		position_ = pos;
	}
	inline void Node::setPosition(int x, int y)
	{
		position_ = Point(x, y);
	}
	inline void Node::setId(size_t id)
	{
		id_ = id;
	}

	class NodePoint: public Node
	{
		public: 
			NodePoint(Net*, size_t id = Node::noid, Point p = Point());
			virtual ~NodePoint();
			virtual Net* getNet() const;
			virtual void toXml(std::ostream&) const;
		private:
			NodePoint(const NodePoint&);
			NodePoint& operator=(const NodePoint&);
		private:
			Net* net_;
	};

	class NodeTerm: public Node
	{
		public:
			NodeTerm(Term*, size_t id = Node::noid);
			virtual ~NodeTerm();
			inline Term* getTerm() const;
			virtual Net* getNet() const;
			virtual void toXml(std::ostream&) const;
		private:
			NodeTerm(const NodeTerm&);
			NodeTerm& operator=(const NodeTerm&);
		private:
			Term* term_;
	};

	inline Term* NodeTerm::getTerm() const { return term_; }
} // Netlist namespace

#endif // NETLIST_NODE_H


#include "Indentation.h"

namespace Netlist
{
	using namespace std;

	Indentation indent; // variable globale
	
	Indentation::Indentation()
		: tabulationSize_(2)
		, indentation_()
	{}

	Indentation& Indentation::operator++()
	{
		indentation_.append(tabulationSize_, ' ');
		return *this;
	}

	Indentation Indentation::operator++(int)
	{
		Indentation before(*this);
		indentation_.append(tabulationSize_, ' ');
		return before;
	}

	Indentation& Indentation::operator--()
	{
		if (indentation_.length() > tabulationSize_) indentation_.resize(indentation_.length() - tabulationSize_);
		else indentation_.clear();
		return *this;
	}
	Indentation Indentation::operator--(int)
	{
		Indentation before(*this);
		if (indentation_.length() > tabulationSize_) indentation_.resize(indentation_.length() - tabulationSize_);
		else indentation_.clear();
		return before;
	}

	ostream& operator<<(ostream& o, const Indentation& indent)
	{
		o << indent.indentation_;
		return o;
	}

} // Netlist namespace


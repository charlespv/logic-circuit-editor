#include "Box.h"

namespace Netlist
{
	using namespace std;
	Box& Box::merge(const Box& other)
	{
		if (not other.isEmpty())
		{
			if (isEmpty())
			{
				x1_ = other.x1_;
				y1_ = other.y1_;
				x2_ = other.x2_;
				y2_ = other.y2_;
			}
			else
			{
				x1_ = min(x1_, other.x1_);
				y1_ = min(y1_, other.y1_);
				x2_ = max(x2_, other.x2_);
				y2_ = max(y2_, other.y2_);
			}
		}
		return *this;
	}
} // Netlist namespace.

#pragma once
#include "point.h"
#include "std_include.h"

namespace jamiti
{
	class Line
	{
	public:
		Line(const Point& start_pt, const Point& end_pt);
		Line(double start_x, double start_y, double end_x, double end_y);
		~Line() = default;

		std::vector<Point> intersect(const Line& other) const;

		double sqr_length() const;

		Point m_start_pt;
		Point m_end_pt;
	};
} //namespace jamiti

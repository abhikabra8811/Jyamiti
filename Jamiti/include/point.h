#pragma once
#include "std_include.h"

namespace jamiti
{
	class Point
	{
	public:

		Point():
			m_x(std::numeric_limits<double>::quiet_NaN()),
			m_y(std::numeric_limits<double>::quiet_NaN())
		{
		}

		Point(double x, double y)
			:m_x(x),
			m_y(y)
		{
			if (!std::isfinite(m_x) || !std::isfinite(m_y))
				throw std::invalid_argument("");
		}

		double square_distance(const Point& other) const;

		double distance(const Point& other) const;

		double dot_product(const Point& other) const;

		double magnitude_sqr() const;


		//Point cross_product(const Point& other) const;
		Point operator+(const Point& other) const;
		Point operator-(const Point& other) const;
		Point operator*(double scalar) const;

		double m_x;
		double m_y;
	};

	//Point operator*(double t, const Point& point)
	//{
	//	return point * t;
	//}
} //namespace jamiti

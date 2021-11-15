#include "..\include\point.h"

namespace jamiti
{
	double jamiti::Point::square_distance(const Point& other) const
	{
		return (other.m_x - m_x) * (other.m_x - m_x) + (other.m_y - m_y) * (other.m_y - m_y);
	}

	double jamiti::Point::distance(const Point& other) const
	{
		return std::sqrt(square_distance(other));
	}

	Point Point::operator+(const Point& other) const
	{
		return Point(m_x + other.m_x, m_y + other.m_y);
	}

	Point Point::operator-(const Point& other) const
	{
		return Point(m_x - other.m_x, m_y - other.m_y);
	}

	Point Point::operator*(double scalar) const
	{
		return Point(m_x * scalar, m_y * scalar);
	}
	
	double Point::dot_product(const Point& other) const
	{
		return m_x * other.m_x + m_y * other.m_y;
	}

	double Point::magnitude_sqr() const
	{
		return m_x * m_x + m_y * m_y;
	}

	//Point Point::cross_product(const Point& other) const
	//{
	//	return Point(); //m_x* other.m_y - m_y * other.m_x;
	//}
} //namespace jamiti

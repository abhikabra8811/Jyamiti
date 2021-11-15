#include "..\include\line.h"

namespace jamiti
{
	Line::Line(const Point& start_pt, const Point& end_pt)
		:m_start_pt(start_pt),
		m_end_pt(end_pt)
	{
	}

	Line::Line(double start_x, double start_y, double end_x, double end_y)
		: m_start_pt(start_x, start_y),
		m_end_pt(end_x, end_y)
	{
	}

	//https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
	std::vector<Point> Line::intersect(const Line& other) const
	{
		//this line P1 + t (P2 - P1)
		//other line Q1 + u (Q2 - Q1)
		Point q1_minus_p1 = other.m_start_pt - m_start_pt;

		Point p2_minus_p1 = m_end_pt - m_start_pt;
		Point q2_minus_q1 = other.m_end_pt - other.m_start_pt;

		double determinant = p2_minus_p1.m_x * q2_minus_q1.m_y - p2_minus_p1.m_y * q2_minus_q1.m_x;

		double l1_sqr = p2_minus_p1.magnitude_sqr();
		double l2_sqr = q2_minus_q1.magnitude_sqr();
		
		double qp_cross_l1 = q1_minus_p1.m_x * p2_minus_p1.m_y - q1_minus_p1.m_y * p2_minus_p1.m_x;
		double qp_cross_l2 = q1_minus_p1.m_x * q2_minus_q1.m_y - q1_minus_p1.m_y * q2_minus_q1.m_x;

		constexpr double eps = 4 * std::numeric_limits<double>::epsilon();
		
		std::vector<Point> intersection_pts;
		[[likely]]
		if (std::abs(determinant) > eps * l1_sqr * l2_sqr)
		{
			//line not parallel
			double t = qp_cross_l2 / determinant;
			if (t >= 0 && t <= 1)
			{
				intersection_pts.push_back(m_start_pt + p2_minus_p1 * t);
			}
		}
		else
		{
			double pq_len_sqr = q1_minus_p1.magnitude_sqr();

			//line parallel
			if (qp_cross_l2 <= eps * l2_sqr * pq_len_sqr)
			{
				//lines are collinear
				double t_l2_start = q1_minus_p1.dot_product(p2_minus_p1) / p2_minus_p1.dot_product(p2_minus_p1);
				double t_l2_end = t_l2_start + q2_minus_q1.dot_product(p2_minus_p1) / p2_minus_p1.dot_product(p2_minus_p1);

				if (t_l2_start > t_l2_end)
					std::swap(t_l2_start, t_l2_end);

				t_l2_start = std::clamp(t_l2_start, 0.0, 1.0);
				t_l2_end = std::clamp(t_l2_end, 0.0, 1.0);
				if (t_l2_start != t_l2_end)
				{
					intersection_pts.push_back(m_start_pt + p2_minus_p1 * t_l2_start);
					intersection_pts.push_back(m_start_pt + p2_minus_p1 * t_l2_end);
				}
			}
			else
			{
				//line parallel but do not intersect
			}
		}
		return intersection_pts;
	}

	double Line::sqr_length() const
	{
		return (m_end_pt.m_x - m_start_pt.m_x) * (m_end_pt.m_x - m_start_pt.m_x) +
						(m_end_pt.m_y - m_start_pt.m_y) * (m_end_pt.m_y - m_start_pt.m_y);
	}
}


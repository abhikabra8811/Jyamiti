#include "..\include\line.h"
#include "..\include\sweep_line.h"
#include "sweep_line_impl.h"

namespace jamiti
{
	SweepLine::SweepLine()
		:m_impl(std::make_unique<SweepLineImpl>())
	{
	}

	SweepLine::~SweepLine()
	{
	}

	void SweepLine::load_geometry(std::vector<Line> segments)
	{
		m_impl->load_geometry(std::move(segments));
	}

	void SweepLine::execute()
	{
		m_impl->execute();
	}
	std::vector<SweepLine::Intersection> SweepLine::get_intersections() const
	{
		return m_impl->get_intersectoion();
	}
} //namespace jamiti
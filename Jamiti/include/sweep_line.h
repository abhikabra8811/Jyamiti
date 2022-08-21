#pragma once
#include "line.h"
namespace jamiti
{
	class SweepLineImpl;
	class SweepLine
	{
	public:
		struct Intersection
		{
			Point m_point;
			size_t index_1;
			size_t index_2;
		};
		SweepLine();
		~SweepLine();

		void load_geometry(std::vector<Line> segments);
		void execute();
		std::vector<Intersection> get_intersections() const;
	private:		
		std::unique_ptr<SweepLineImpl> m_impl;
	};
} //namespace jamiti

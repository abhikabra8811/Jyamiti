#pragma once
#include "line.h"
namespace jamiti
{
	class SweepLineImpl;
	class SweepLine
	{
	public:
		SweepLine();
		~SweepLine();

		void load_geometry(std::vector<Line> segments);
		void execute();
	private:		
		std::unique_ptr<SweepLineImpl> m_impl;
	};
} //namespace jamiti

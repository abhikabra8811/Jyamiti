#pragma once
#include "line.h"
namespace jamiti
{
	class SweepLine
	{
	public:
		SweepLine();
		~SweepLine();

		void load_geometry(std::vector<Line> segments);

		void execute();

		class Impl;
	private:		
		std::unique_ptr<Impl> m_impl;
	};
} //namespace jamiti

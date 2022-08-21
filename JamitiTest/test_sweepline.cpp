#include "pch.h"

using namespace jamiti;

TEST(TestSweepline, LoadSegments)
{
	std::vector<jamiti::Line> segments;
	segments.push_back(Line(0, 0, 2, 2));
	segments.push_back(Line(0, 2, 2, 0));
	segments.push_back(Line(1, 0, 2, 1));
	SweepLine sweep_line;
	sweep_line.load_geometry(std::move(segments));
	sweep_line.execute();
	auto intersections = sweep_line.get_intersections();
}
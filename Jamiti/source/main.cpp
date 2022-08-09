#include "..\include\jamiti_inclue.h"

using namespace jamiti;
int main()
{
	//{
	//	jamiti::Line l1(0, 0, 1, 1);
	//	Line l2(0, 1, 1, 0);

	//	auto intersections = l1.intersect(l2);
	//}

	//{
	//	jamiti::Line l1(0, 0, 1, 0);
	//	Line l2(5, 5, 5, -5);
	//	auto intersections = l1.intersect(l2);
	//}

	//{
	//	jamiti::Line l1(0, 0, 2, 2);
	//	Line l2(1, 1, 3, 3);
	//	auto intersections = l1.intersect(l2);
	//}

	//{
	//	jamiti::Line l1(0, 0, 1, 1);
	//	Line l2(-1, -1, 2, 2);
	//	auto intersections = l1.intersect(l2);
	//}

	{
		std::vector<jamiti::Line> segments;
		segments.push_back(Line(0, 0, 2, 2));
		segments.push_back(Line(0, 2, 2, 0));
		segments.push_back(Line(1, 0, 2, 1));
		SweepLine sweep_line;
		sweep_line.load_geometry(std::move(segments));
		sweep_line.execute();
	}

	return 0;
}
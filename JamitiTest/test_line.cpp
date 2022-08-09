#include "pch.h"

using jamiti::Line;
using jamiti::Point;

TEST(TestCaseName, TestName) {
	{
		jamiti::Line l1(0, 0, 1, 1);
		Line l2(0, 1, 1, 0);
		auto intersections = l1.intersect(l2);
		EXPECT_EQ(intersections.size(), 1);
	}

	{
		jamiti::Line l1(0, 0, 1, 0);
		Line l2(5, 5, 5, -5);
		auto intersections = l1.intersect(l2);
	}

	{
		jamiti::Line l1(0, 0, 2, 2);
		Line l2(1, 1, 3, 3);
		auto intersections = l1.intersect(l2);
	}

	{
		jamiti::Line l1(0, 0, 1, 1);
		Line l2(-1, -1, 2, 2);
		auto intersections = l1.intersect(l2);
	}

  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}
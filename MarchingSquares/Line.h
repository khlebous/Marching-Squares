#pragma once

#include "Point.h"

class Line
{
public:
	Line() {};
	~Line() {};

public:
	Line(Point a, Point b)
	{
		A = a;
		B = b;
	}

	Point A;
	Point B;

	bool operator==(const Line& l) const
	{
		return (A == l.A && B == l.B) || (A == l.B && B == l.A);
	}
};


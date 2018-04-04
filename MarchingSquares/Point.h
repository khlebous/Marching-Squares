#pragma once
class Point
{
public:
	int x;
	int y;

	Point() {};
	Point(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	bool operator==(const Point& p) const
	{
		return x == p.x && y == p.y;
	}
};


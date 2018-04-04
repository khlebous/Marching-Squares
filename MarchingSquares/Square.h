#pragma once

#include <vector>
#include "Line.h"

using namespace std;
class Square
{
private:
	int _number;
	
	Point _downRightCorner;
	Point TopLeft;
	Point DownRight;
	Point TopRight;
	Point DownLeft;
	Point TopMid;
	Point DownMid;
	Point LeftMid;
	Point RightMid;

public:
	Square() {};
	~Square() {};

	Square(int number, Point topLeftCorner, Point downRightCorner)
	{
		TopLeft = topLeftCorner;
		DownRight = downRightCorner;
		_number = number;

		TopRight = Point(DownRight.x, TopLeft.y);
		DownLeft = Point(TopLeft.x, DownRight.y);
		TopMid = Point((TopLeft.x + TopRight.x) / 2, TopRight.y);
		DownMid = Point((DownLeft.x + DownRight.x) / 2, DownRight.y);
		LeftMid = Point(TopLeft.x, (DownLeft.y + TopLeft.y) / 2);
		RightMid = Point(TopRight.x, (DownRight.y + TopRight.y) / 2);
	}

	vector<Line> GetLines()
	{
		switch (_number)
		{
			case 1: return GetLines(vector<int>(1, 4));
			case 2: return GetLines(vector<int>(1, 3));
			case 3: return GetLines(vector<int>(1, 6));
			case 4: return GetLines(vector<int>(1, 2));
			case 5: return GetLines({ 1, 3 });
			case 6: return GetLines(vector<int>(1, 5));
			case 7: return GetLines(vector<int>(1, 1));
			case 8: return GetLines(vector<int>(1, 1));
			case 9: return GetLines(vector<int>(1, 5));
			case 10: return GetLines({ 2, 4 });
			case 11: return GetLines(vector<int>(1, 2));
			case 12: return GetLines(vector<int>(1, 6));
			case 13: return GetLines(vector<int>(1, 3));
			case 14: return GetLines(vector<int>(1, 4));
			default: return GetLines(vector<int>());

		}
	}

private:
	vector<Line> GetLines(vector<int> nrs)
	{
		vector<Line> out = vector<Line>();
		for (int nr : nrs)
			out.push_back(GetLines(nr));
		return out;
	}

	Line GetLines(int nr)
	{
		switch (nr)
		{
			case 1: return Line(LeftMid, TopMid);
			case 2: return Line(TopMid, RightMid);
			case 3: return Line(RightMid, DownMid);
			case 4: return Line(DownMid, LeftMid);
			case 5: return Line(TopMid, DownMid);
			case 6: return Line(LeftMid, RightMid);
			default: throw;
		}
	}
};


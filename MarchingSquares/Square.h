#pragma once

#include <vector>
#include "Line.h"

using namespace std;
class Square
{
public:
	Square() {};
	~Square() {};

private:
	Point _downRightCorner;
	int _number;

	Point TopLeft;
	Point DownRight;
	Point TopRight;
	Point DownLeft;
	Point TopMid;
	Point DownMid;
	Point LeftMid;
	Point RightMid;

public:
	Square(int number, Point topLeftCorner, Point downRightCorner)
	{
		TopLeft = topLeftCorner;
		DownRight = downRightCorner;
		_number = number;

		TopRight = Point(DownRight.x, TopLeft.y);
		DownLeft =  Point(TopLeft.x, DownRight.y);
		TopMid = Point((TopLeft.x + TopRight.x) / 2, TopRight.y);
		DownMid = Point((DownLeft.x + DownRight.x) / 2, DownRight.y);
		LeftMid =  Point(TopLeft.x, (DownLeft.y + TopLeft.y) / 2);
		RightMid =  Point(TopRight.x, (DownRight.y + TopRight.y) / 2);
	}

	vector<Line> GetLines()
	{
		switch (_number)
		{
		case 0: return GetLines(vector<int>());
		case 1: return GetLines( { 6, 7, 12 });
		case 2: return GetLines( { 4, 5, 11 });
		case 3: return GetLines( { 4, 5, 6, 7, 14 });
		case 4: return GetLines( { 2, 3, 10 });
		case 5: return GetLines( { 2, 3, 6, 7, 9, 11 });
		case 6: return GetLines( { 2, 3, 4, 5, 13 });
		case 7: return GetLines( { 2, 3, 4, 5, 6, 7, 9 });
		case 8: return GetLines( { 1, 8, 9 });
		case 9: return GetLines( { 1, 6, 7, 8, 13 });
		case 10: return GetLines( { 1, 4, 5, 8, 10, 12 });
		case 11: return GetLines( { 1, 4, 5, 6, 7, 8, 10 });
		case 12: return GetLines( { 1, 2, 3, 8, 14 });
		case 13: return GetLines( { 1, 2, 3, 6, 7, 8, 11 });
		case 14: return GetLines( { 1, 2, 3, 4, 5, 8, 12 });
		case 15: return GetLines( { 1, 2, 3, 4, 5, 6, 7, 8 });
		default: throw;
		}
	}

private:
	vector<Line> GetLines(vector<int> nrs)
	{
		vector<Line> out = vector<Line>();
		for (int nr : nrs)
		{
			out.push_back(GetLines(nr));
		}
		return out;
	}

	Line GetLines(int nr)
	{
		switch (nr)
		{
		case 1: return Line(TopLeft, TopMid);
		case 2: return Line(TopMid, TopRight);
		case 3: return Line(TopRight, RightMid);
		case 4: return Line(RightMid, DownRight);
		case 5: return Line(DownRight, DownMid);
		case 6: return Line(DownMid, DownLeft);
		case 7: return Line(DownLeft, LeftMid);
		case 8: return Line(LeftMid, TopLeft);
		case 9: return Line(LeftMid, TopMid);
		case 10: return Line(TopMid, RightMid);
		case 11: return Line(RightMid, DownMid);
		case 12: return Line(DownMid, LeftMid);
		case 13: return Line(TopMid, DownMid);
		case 14: return Line(LeftMid, RightMid);
		default: throw;
		}
	}
};


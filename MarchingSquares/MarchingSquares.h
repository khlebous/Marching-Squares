#pragma once
#include "Point.h"
#include "Square.h"

extern int SCR_WIDTH;
extern int SCR_HEIGHT;
extern int STEP;
extern int THRESHHOLD;

using namespace std;
namespace MS
{
	inline pair<Point, bool>** GetSamples(unsigned char* data)
	{
		size_t h = (SCR_HEIGHT - 1) / STEP + 1;

		pair<Point, bool>**  result = new pair<Point, bool>*[h];
		for (size_t i = 0; i < h; i++)
			result[i] = new pair<Point, bool>[(SCR_WIDTH - 1) / STEP + 1];

		for (int i = 0; i * STEP < SCR_HEIGHT; i++)
		{
			for (int j = 0; j * STEP < SCR_WIDTH; j++)
			{
				int colour = (data[i * STEP * 3 * SCR_WIDTH + j * STEP * 3] +
					data[i * STEP * 3 * SCR_WIDTH + j * STEP * 3 + 1] +
					data[i * STEP * 3 * SCR_WIDTH + j * STEP * 3 + 2]) / 3;
				bool value = colour <= THRESHHOLD;
				auto point = make_pair(Point(i * STEP, j * STEP), value);
				result[i][j] = point;
			}
		}

		return result;
	}

	Square** GetSquares(pair<Point, bool>** samples)
	{
		size_t w = (SCR_WIDTH - 1) / STEP + 1;
		size_t h = (SCR_HEIGHT - 1) / STEP + 1;

		Square** squares = new Square*[h];
		for (size_t i = 0; i < h; i++)
			squares[i] = new Square[w];

		for (size_t i = 0; i < h - 1; i++)
			for (size_t j = 0; j < w - 1; j++)
			{
				int value = 0;
				if (samples[i][j].second) value += 8;
				if (samples[i + 1][j].second) value += 4;
				if (samples[i + 1][j + 1].second) value += 2;
				if (samples[i][j + 1].second) value += 1;

				squares[i][j] = Square(value, samples[i][j].first, samples[i + 1][j + 1].first);
			}

		for (size_t i = 0; i < h; i++)
			delete[] samples[i];
		delete[] samples;

		return squares;
	}

	vector<Line> GetLines(Square**  squares)
	{
		vector<Line> lines = vector<Line>();

		size_t w = (SCR_WIDTH - 1) / STEP + 1;
		size_t h = (SCR_HEIGHT - 1) / STEP + 1;

		for (size_t i = 0; i < h - 1; i++)
		{
			for (size_t j = 0; j < w - 1; j++)
			{
				Square* square = &squares[i][j];
				for (Line &line : (*square).GetLines())
				{
					size_t lineIndex = distance(lines.begin(), find(lines.begin(), lines.end(), line));
					if (lineIndex >= lines.size())
						lines.push_back(line);
					else
						lines.erase(lines.begin() + lineIndex);
				}
			}
		}

		for (size_t i = 0; i < h; i++)
			delete[] squares[i];
		delete[] squares;
		return lines;
	}

	vector<Line> MarchingSquares(unsigned char * data)
	{
		auto samples = GetSamples(data);
		auto squares = GetSquares(samples);
		auto lines = GetLines(squares);
		return lines;
	}
}
#pragma once

#include "Common.h"
#include "Point.h"

NS_BF_BEGIN;

template <typename T>
class Rect
{
public:
	T x;
	T y;
	T width;
	T height;

public:
	Rect()
	{
		x = 0;
		y = 0;
		width = 0;
		height = 0;
	}

	Rect(T x, T y, T width, T height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	bool operator!=(const Rect& r2)
	{
		return (x != r2.x) || (y != r2.y) || (width != r2.width) || (height != r2.height);
	}

	bool Contains(T x, T y)
	{
		return (x >= this->x) && (y >= this->y) && (x < this->x + width) && (y < this->y + height);
	}

	bool Contains(Point<T> pt)
	{
		return (pt.x >= this->x) && (y >= this->y) && (x < this->x + width) && (y < this->y + height);
	}

	T GetRight()
	{
		return x + width;
	}

	T GetBottom()
	{
		return y + height;
	}

	Rect Intersection(Rect rect)
	{
		T x1 = Max(x, rect.x);
		T x2 = Min(x + width, rect.x + rect.width);
		T y1 = Max(y, rect.y);
		T y2 = Min(y + height, rect.y + rect.height);
		if (((x2 - x1) < 0) || ((y2 - y1) < 0))
			return Rect();
		else
			return Rect(x1, y1, x2 - x1, y2 - y1);
	}

	Rect Union(Rect rect)
	{
		T x1 = Min(x, rect.x);
		T x2 = Max(x + width, rect.x + rect.width);
		T y1 = Min(y, rect.y);
		T y2 = Max(y + height, rect.y + rect.height);
		return Rect(x1, y1, x2 - x1, y2 - y1);
	}

	void Include(Point<T> pt)
	{
		T left = x;
		T top = y;
		T right = x + width;
		T bottom = y + height;
		x = BF_MIN(pt.x, left);
		y = BF_MIN(pt.y, top);
		width = BF_MAX(pt.x, right) - x;
		height = BF_MAX(pt.y, bottom) - y;
	}
};

typedef Rect<double> RectD;
typedef Rect<int32> RectI32;

NS_BF_END;
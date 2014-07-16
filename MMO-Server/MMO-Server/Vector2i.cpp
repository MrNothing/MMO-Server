#include "stdafx.h"
#include "Vector2i.h"
#include <math.h>

Vector2i::Vector2i(int _x, int _y)
{
	x = _x;
	y = _y;
}

Vector2i::Vector2i()
{
	x = 0;
	y = 0;
}


Vector2i::~Vector2i(void)
{
}


int Vector2i::getDistance(Vector2i p2)
{
	return sqrt((x-p2.x)*(x-p2.x)+(y-p2.y)*(y-p2.y));
}

Vector2i Vector2i::operator-(Vector2i p2)
{
	Vector2i diff;
	diff.x = x-p2.x;
	diff.y = y-p2.y;
	return diff;
}

Vector2i Vector2i::operator+(Vector2i p2)
{
	Vector2i diff;
	diff.x = x+p2.x;
	diff.y = y+p2.y;
	return diff;
}

Vector2i Vector2i::operator*(int val)
{
	Vector2i diff;
	diff.x=x*val;
	diff.y=y*val;
	return diff;
}
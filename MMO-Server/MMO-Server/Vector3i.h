#pragma once
#include "vector2i.h"
#include <string>
class Vector3i :
	public Vector2i
{
public:
	Vector3i(void);
	Vector3i(int _x, int _y, int _z);
	~Vector3i(void);
	int z;
	float getDistance(Vector3i p2);
	Vector3i operator-(Vector3i p2);
	Vector3i operator+(Vector3i p2);
	Vector3i operator*(int val);
	Vector3i operator/(int val);
	Vector3i operator*(Vector3i p2);
	Vector3i normalized();
	Vector3i flatten(float step);
	float magnitude();
	float sqrMagnitude();
	std::string toString();
};


#pragma once
#include "vector2.h"
#include <string>
class Vector3 :
	public Vector2
{
public:
	Vector3(void);
	Vector3(float _x, float _y, float _z);
	~Vector3(void);
	float z;
	float getDistance(Vector3 p2);
	Vector3 operator-(Vector3 p2);
	Vector3 operator+(Vector3 p2);
	Vector3 operator*(float val);
	Vector3 operator/(float val);
	Vector3 operator*(Vector3 p2);
	Vector3 normalized();
	Vector3 flatten(float step);
	float magnitude();
	float sqrMagnitude();
	std::string toString();
};


#pragma once
class Vector2
{
public:
	Vector2(void);
	Vector2(float _x, float _y);
	~Vector2(void);
	float x,y;
	float getDistance(Vector2 p2);
	Vector2 operator-(Vector2 p2);
	Vector2 operator+(Vector2 p2);
	Vector2 operator*(float val);
};


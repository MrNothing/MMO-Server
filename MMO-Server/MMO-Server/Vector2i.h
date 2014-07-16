#pragma once
class Vector2i
{
public:
	Vector2i(void);
	Vector2i(int _x, int _y);
	~Vector2i(void);
	int x,y;
	int getDistance(Vector2i p2);
	Vector2i operator-(Vector2i p2);
	Vector2i operator+(Vector2i p2);
	Vector2i operator*(int val);
};


#pragma once

struct Vector2
{
	inline Vector2() : x(0), y(0)
	{
	}
	inline Vector2(float _x, float _y) : x(_x), y(_y)
	{
	}

	inline Vector2 operator + (const Vector2 &other)
	{
		return Vector2(x + other.x, y + other.y);
	}

	inline Vector2 operator - (const Vector2 &other)
	{
		return Vector2(x - other.x, y - other.y);
	}

	inline Vector2 operator * (const float multiplier)
	{
		return Vector2(x * multiplier, y * multiplier);
	}

	inline Vector2 operator / (const Vector2 &other)
	{
		return Vector2(x / other.x, y / other.y);
	}

	inline Vector2 operator / (const int &divisor)
	{
		return Vector2(x / divisor, y / divisor);
	}

	inline void operator += (const Vector2 &other)
	{
		x += other.x;
		y += other.y;
		return;
	}

	inline void operator *= (const Vector2 &other)
	{
		x *= other.x;
		y *= other.y;
		return;
	}

	inline bool operator == (const Vector2 other)
	{
		return (x == other.x) && (y == other.y);
	}

	float x;
	float y;
};
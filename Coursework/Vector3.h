#pragma once

struct Vector3
{
	inline Vector3() : x(0), y(0), z(0)
	{
	}
	inline Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{
	}

	inline Vector3 operator + (const Vector3 &other)
	{
		return Vector3(x + other.x, y + other.y, z + other.z);
	}

	inline Vector3 operator - (const Vector3 &other)
	{
		return Vector3(x - other.x, y - other.y, z - other.z);
	}

	inline Vector3 operator * (const float multiplier)
	{
		return Vector3(x * multiplier, y * multiplier, z * multiplier);
	}

	inline Vector3 operator / (const Vector3 &other)
	{
		return Vector3(x / other.x, y / other.y, z / other.z);
	}

	inline Vector3 operator / (const int &divisor)
	{
		return Vector3(x / divisor, y / divisor, z / divisor);
	}

	inline void operator += (const Vector3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return;
	}

	inline void operator *= (const Vector3 &other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return;
	}

	inline bool operator == (const Vector3 other)
	{
		return (x == other.x) && (y == other.y) && (z == other.z);
	}

	float x;
	float y;
	float z;
};
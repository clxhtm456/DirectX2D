#include "stdafx.h"
#include "Math.h"

const float Math::PI = 3.141592f;

float Math::ToRadian(float degree)
{
	return degree * PI/180.0f;
}

float Math::ToDegree(float radian)
{
	return radian* 180.0f/PI;
}

int Math::Random(int r1, int r2)
{
	auto value = (int)(rand() % (r2 - r1 + 1) + r1);
	return value;
}

float Math::Random(float r1, float r2)
{
	auto random = (float)rand() / (float)RAND_MAX;
	auto diff = r2 - r1;
	float val = random * diff;
	//0xFF//1BYTE
	return r1+val;
}

void Math::RandomCirclePoint(float halfLength, OUT float & x, OUT float & y)
{
	float resultX = Random(-halfLength, halfLength);

	float yLength = sqrt(pow(halfLength, 2) - pow(resultX, 2));
	float resultY = Random(-yLength, yLength);

	x = resultX;
	y = resultY;
}

float Math::Cross(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2)
{
	return (vec1.x * vec2.y) - (vec1.y * vec2.x);
}

float Math::Ccw(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2)
{
	return Cross(vec1,vec2);
}

float Math::Ccw(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2, D3DXVECTOR2 vec3)
{
	return Ccw((vec2 - vec1),(vec3 - vec1));
}

float Math::Distance(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2)
{
	auto distance  = sqrt(pow(vec1.x - vec2.x, 2) + pow(vec1.y - vec2.y, 2));
	return distance;
}


bool Math::SementInterSect(D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c, D3DXVECTOR2 d)
{
	double ab = Ccw(a, b, c) * Ccw(a, b, d);
	double cd = Ccw(c, d, a) * Ccw(c, d, b);

	if (ab == 0 && cd == 0)
	{
		if (b < a) swap(a, b);
		if (d < c) swap(c, d);

		return !(b < c || d < a);
	}

	return ab <= 0 && cd <= 0;
}


D3DXVECTOR2 Math::MoveTowards(D3DXVECTOR2 current, D3DXVECTOR2 b, float maxDistanceDelta)
{
	D3DXVECTOR2 a = b - current;
	float magnitude = Distance(b, current);
	if (magnitude <= maxDistanceDelta || magnitude == 0.0f)
	{
		return b;
	}
	return current + a / magnitude * maxDistanceDelta;
}

D3DXVECTOR2 Math::Lerp(D3DXVECTOR2 current, D3DXVECTOR2 b, float maxDistanceDelta)
{
	return D3DXVECTOR2();
}

int Math::NumberOfFigure(int value, int digit)
{
	int tenFigure = pow(10, digit);
	int result = (value % tenFigure)/ pow(10, digit-1);
	return result;
}

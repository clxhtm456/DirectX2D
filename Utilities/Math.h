#ifndef __MATH_H__
#define __MATH_H__

enum class InterSectType
{
	TYPE_NONE,
	TYPE_BOTTOM,
	TYPE_WALL,
};

class Math
{
private:
public:
	static const float PI;

	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);
	static void RandomCirclePoint(float halfLength,OUT float &x, OUT float &y);

	static float Cross(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2);
	static float Ccw(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2);
	static float Ccw(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2, D3DXVECTOR2 vec3);

	static float Distance(D3DXVECTOR2 vec1, D3DXVECTOR2 vec2);

	static bool SementInterSect(D3DXVECTOR2 a, D3DXVECTOR2 b, D3DXVECTOR2 c, D3DXVECTOR2 d);
	static D3DXVECTOR2 MoveTowards(D3DXVECTOR2 current, D3DXVECTOR2 b, float maxDistanceDelta);
	static D3DXVECTOR2 Lerp(D3DXVECTOR2 current, D3DXVECTOR2 b, float maxDistanceDelta);

	static int NumberOfFigure(int value, int digit);
};

#endif
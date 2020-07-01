#pragma once

class PhysicsBox
{
public:
	PhysicsBox();
	PhysicsBox(float posX,float posY,float sizeX, float sizeY);
	PhysicsBox(D3DXVECTOR2 position,D3DXVECTOR2 size);
	virtual ~PhysicsBox();

	void Width(float value) { _width = value; }
	void Height(float value) { _height = value; }
	void Size(float x, float y) { _width = x; _height = y; }
	float Width() { return _width; }
	float Height() { return _height; }

	void Position(float x, float y) { _position = D3DXVECTOR2(x,y); }
	void Position(D3DXVECTOR2 value) { _position = value; }
	D3DXVECTOR2 Position() { return _position; }
	void SetSizelikeTexture(Sprite* sprite);
private:
	float _width;
	float _height;
	D3DXVECTOR2 _position;
};
#pragma once

class Fish
{
private:
	Animation * fish;

	D3DXVECTOR2 position;
	D3DXVECTOR2 start;
	float speed;

public:
	Fish(D3DXVECTOR2 position);
	~Fish();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	Sprite* GetSprite();

public:
	void Position(D3DXVECTOR2 val) { start = val; position = val; }
	D3DXVECTOR2 Position() { return position; }

};
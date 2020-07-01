#pragma once

class Hero
{
private:
	Animation * animation;

	D3DXVECTOR2 position;
	float moveSpeed;

public:
	Hero(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~Hero();


	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	Sprite* GetSprite();

	void Position(D3DXVECTOR2 vec) { position = vec; }
	D3DXVECTOR2 Position() { return position; }

};

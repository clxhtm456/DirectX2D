#pragma once

class Hero
{
public:
	Hero(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~Hero();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	Sprite* GetSprite()
	{
		return animation->GetSprite();
	}

	void Position(D3DXVECTOR2 vec) { animation->Position(vec); }
	D3DXVECTOR2 Position() { return animation->Position(); }
private:
	Animation * animation;

	float moveSpeed;
};
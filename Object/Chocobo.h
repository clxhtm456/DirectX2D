#pragma once

class Chocobo
{
private:
	Animation * animation;

	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;

	bool bMove;
	

public:
	Chocobo(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~Chocobo();


	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	Sprite* GetSprite();

	void Position(D3DXVECTOR2 vec) { position = vec; }
	D3DXVECTOR2 Position() { return position; }

	void Scale(D3DXVECTOR2 vec) { scale = vec; }
	D3DXVECTOR2 Scale() { return scale; }

	void Move(bool val) { bMove = val; }
	bool Move() { return bMove; }

};

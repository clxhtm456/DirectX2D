#pragma once

class Mole
{
private:
	Animation * mole;
	D3DXVECTOR2 start;
	D3DXVECTOR2 position;

	float speed;

public:
	Mole(D3DXVECTOR2 position);
	~Mole();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	void Position(D3DXVECTOR2 val) { position = val; start = val; }
	D3DXVECTOR2 Position() { return position; }

};
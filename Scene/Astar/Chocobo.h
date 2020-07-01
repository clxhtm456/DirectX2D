#pragma once

class Chocobo
{
public:
	Chocobo(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~Chocobo();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	Sprite* GetSprite()
	{
		return animation->GetSprite();
	}

	void Position(D3DXVECTOR2 vec) { animation->Position(vec); }
	D3DXVECTOR2 Position() { return animation->Position(); }

	void Scale(D3DXVECTOR2 vec) { animation->Scale(vec); }
	D3DXVECTOR2 Scale() { return animation->Scale(); }

	void Move(bool val) { bMove = val; }
	bool Move() { return bMove; }
	void AutoMove(vector<POINT> waypoint);
private:
	Animation * animation;

	vector<POINT> wayPoint;
	bool startMove;
	int recentStage;
	int lastStage;

	bool bMove;
};
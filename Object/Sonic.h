#pragma once
#include "./Viewer/IFollow.h"

enum class PlayerState
{
	IDLE = 0,
	IDLE_LIE_DOWN,
	NORMAL_RUN,
	FAST_RUN,
	SIT_DOWN,
	TURBO,
	ROLL,
	PUSH
};

class Sonic : public IFollow
{
private:
	Animation * player;
	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;

	D3DXVECTOR2 bottomVector;


	D3DXVECTOR2 focusOffset;
	D3DXVECTOR2 textureSize;

	PlayerState state;

	float moveSpeed;
	float rollSpeed;

	float accel; // x축 가속도
	float velocity; // y축 가속도
	float gravity;

	bool bGround;
	bool direction; //true : 오른쪽, false : 왼쪽

	//라인 충돌 관련
	D3DXVECTOR2 lineLeftPoint;
	D3DXVECTOR2 lineRightPoint;
	bool bLineBottomColision;

	D3DXVECTOR3 degree;
	float slope;

private:
	void MoveLeft();
	void MoveRight();
	void SitDown();


public:
	Sonic(D3DXVECTOR2 position, D3DXVECTOR2 scale);
	~Sonic();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();
	
	void Focus(D3DXVECTOR2 * position, D3DXVECTOR2 * size) override;
	Sprite* GetSprite();

public:
	void Position(D3DXVECTOR2 vec) { position = vec; }
	D3DXVECTOR2 Position() { return position; }

	void Scale(D3DXVECTOR2 vec);
	D3DXVECTOR2 TextureSize() { return textureSize; }

	D3DXVECTOR2 BottomVector() { return bottomVector; }

	void LineBottomCollision(bool val) { bLineBottomColision = val; }

	void SetPlayerState(PlayerState val) { state = val; }
	PlayerState GetPlayerState() { return state; }

	bool OnGround() { return bGround; }

	//라인 정보 Get, Set
public:
	void LineLeftPoint(D3DXVECTOR2 val) { lineLeftPoint = val; }
	void LineRightPoint(D3DXVECTOR2 val) { lineRightPoint = val; }
};

#pragma once

#include "Scene/Scene.h"

class AstarScene : public Scene
{
public:
	AstarScene();
	~AstarScene();

	void RenderImGui();
	void RenderDebug();

	void EditTile();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render() override;
private:
	class Astar* pathFinder;

	Sprite* backGroundImg;
	class Hero* player;
	class Chocobo* enemy;

	bool bMove;

	//�̹��� ��ư
	vector<class TileMagma*> magmas;
	ID3D11ShaderResourceView* srv;
	bool bButton;

	D3DXVECTOR2 mouse;
	D3DXVECTOR2 position;
	D3DXVECTOR2 cameraPos;

	
};
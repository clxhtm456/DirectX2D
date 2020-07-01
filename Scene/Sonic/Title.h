#pragma once

#include "Scene/Scene.h"

class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;

private:
	Clip* sonic;
	Clip* tales;
	Sprite* sky;
	Sprite* island;
	Sprite* badge;

	D3DXMATRIX view;

	
};
#pragma once
#include "Scene.h"

class Stage1 : public Scene
{
private:
	
	Sprite* background;

	class Player* player;
	class Bullet* bullet;
	class Fire* fire;

private:
	void RenderBackground();

public:
	Stage1(SceneValues* values);
	~Stage1();

	void Update() override;
	void Render() override;

	class Player* GetPlayer();
};
#pragma once

#include "Scene/Scene.h"

class Bloom : public Scene
{
public:
	Bloom();
	~Bloom();

	// Scene을(를) 통해 상속됨
	void Update() override;
	void Render() override;

private:
	class Player* player;

};
#pragma once

#include "Scene/Scene.h"

class Bloom : public Scene
{
public:
	Bloom();
	~Bloom();

	// Scene��(��) ���� ��ӵ�
	void Update() override;
	void Render() override;

private:
	class Player* player;

};
#pragma once

#include "Game/Gun/Weapon.h"

class Bazooka : public Weapon
{
public:
	static Bazooka* Create();
	bool Init();

	Bullet* CreateBullet();
	// Weapon��(��) ���� ��ӵ�
	virtual void Shot() override;
};
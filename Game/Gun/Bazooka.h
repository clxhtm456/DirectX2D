#pragma once

#include "Game/Gun/Weapon.h"

class Bazooka : public Weapon
{
public:
	static Bazooka* Create();
	bool Init();

	Bullet* CreateBullet();
	// Weapon을(를) 통해 상속됨
	virtual void Shot() override;
};
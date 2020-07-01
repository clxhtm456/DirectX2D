#pragma once

#include "Weapon.h"

class HandGun : public Weapon
{
public:
	static HandGun* Create();
	bool Init();

	Bullet* CreateBullet();

	// Weapon을(를) 통해 상속됨
	virtual void Shot() override;

};
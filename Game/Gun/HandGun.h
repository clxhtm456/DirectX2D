#pragma once

#include "Weapon.h"

class HandGun : public Weapon
{
public:
	static HandGun* Create();
	bool Init();

	Bullet* CreateBullet();

	// Weapon��(��) ���� ��ӵ�
	virtual void Shot() override;

};
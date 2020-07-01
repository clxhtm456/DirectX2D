#pragma once

#include "Vehicle.h"
#include "Bullet.h"

class SlugTank : public Vehicle,public IBulletManager
{
public:
	static SlugTank* Create();
	bool Init();
private:

	// IBulletManager을(를) 통해 상속됨
	virtual float GetAngle() override;

	// IBulletManager을(를) 통해 상속됨
	virtual void ActiveBullet() override;
	virtual void DeactiveBullet() override;
};
#pragma once

#include "Vehicle.h"
#include "Bullet.h"

class SlugTank : public Vehicle,public IBulletManager
{
public:
	static SlugTank* Create();
	bool Init();
private:

	// IBulletManager��(��) ���� ��ӵ�
	virtual float GetAngle() override;

	// IBulletManager��(��) ���� ��ӵ�
	virtual void ActiveBullet() override;
	virtual void DeactiveBullet() override;
};
#pragma once

#include "Weapon.h"

class HMachineGun : public Weapon
{
public:
	static HMachineGun* Create();
	bool Init();
	Bullet* CreateBullet();
	// Weapon을(를) 통해 상속됨
	virtual void Shot() override;
	void MoveAngle(float angle) override;
	float GetAngle() override;
	void Update();
private:
	void UpdateAngle();
	void ShotMechanism();
	void CreateHBullet();

	bool active;
	int step;
	float delay;

	int angleCount;
	bool trigger;

	float goalAngle;
};
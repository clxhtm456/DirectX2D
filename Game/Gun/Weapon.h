#pragma once

#include "Game/Unit/Bullet.h"
#include "Game/UI/MetalUI.h"

class Weapon : public Node,public IBulletManager
{
public:
	virtual ~Weapon();
	void ClearBullet();
	bool Init();

	void StartShoot();
	
	virtual void MoveAngle(float angle);
	virtual void SetAngle(float angle);
	// IBulletManager을(를) 통해 상속됨
	virtual float GetAngle() override;
	virtual void DeactiveBullet() override;
	virtual void ActiveBullet() override;

	bool CanFire();
	void SetCooldownTime(float value) { cooldownTime = value; }

	void SetFlag(int value) { bulletFlag = value; }
	int GetFlag() { return bulletFlag; }

	void SetZeroAmmoCallback(const std::function<void()>& callback);

	D3DXVECTOR2 WeaponPos();
private:
	int ammo;
protected:
	vector<Bullet*> bulletList;
	virtual void Shot() = 0;
	float shotAngle;
	float offset;

	int activeAmmo;
	float bulletSpeed;

	float cooldownTime;
	float cooldownTimer;
	bool canFire;

	int bulletFlag;

	std::function<void()> zeroAmmoCallback;

	void SetAmmo(int val);
	int GetAmmo() { return ammo; }
protected:
	virtual Bullet* CreateBullet();
	Bullet* SearchBullet();
private:
	IAmmoObserver* ammoObserver;
public:
	void SetAmmoObserver(IAmmoObserver* observer);
};
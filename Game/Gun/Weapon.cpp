#include "stdafx.h"
#include "Weapon.h"

Weapon::~Weapon()
{
}

void Weapon::ClearBullet()
{
	for (auto bullet : bulletList)
	{
		if (bullet->GetRunning() == false)
		{
			bullet->RemoveSelf();
		}
		else
		{
			bullet->RemoveManager();
		}
	}
}

bool Weapon::Init()
{
	if (!Node::Init())
		return false;

	shotAngle = 0.0f;
	offset = 40.0f;
	activeAmmo = 0;

	canFire = false;
	cooldownTime = 0.5f;
	cooldownTimer = 0.0f;

	zeroAmmoCallback = NULL;

	return true;
}

void Weapon::StartShoot()
{
	if (ammo != 0)
	{
		Shot();
	}
}

void Weapon::MoveAngle(float degree)
{
	SetAngle(degree);
}

void Weapon::SetAngle(float degree)
{
	shotAngle = degree;
}

float Weapon::GetAngle()
{
	return shotAngle;
}

void Weapon::SetAmmo(int val)
{
	ammo = val;
	//notify
	if(ammoObserver != nullptr)
		ammoObserver->AmmoCalc(val);
}

Bullet * Weapon::CreateBullet()
{
	auto bullet = Bullet::Create(this);
	bullet->Flag(bulletFlag);
	
	return bullet;
}

Bullet* Weapon::SearchBullet()
{
	int size = bulletList.size();
	if (size <= activeAmmo)
	{
		auto bullet = CreateBullet();
		Director::getInstance()->GetRunningScene()->AddChild(bullet,4);
		bulletList.push_back(bullet);
		bullet->Position(WeaponPos());
		bullet->SetMoveSpeed(bulletSpeed);
		bullet->RotationDegree(0, 0, GetAngle());
		return bullet;
	}
	else
	{
		for (auto bullet : bulletList)
		{
			if (!bullet->GetRunning())
			{
				bullet->Position(WeaponPos());
				bullet->SetMoveSpeed(bulletSpeed);
				bullet->RotationDegree(0, 0, GetAngle());
				return bullet;
			}
				
		}
	}
}

void Weapon::SetAmmoObserver(IAmmoObserver* observer)
{
	ammoObserver = observer;
	SetAmmo(GetAmmo());
}

void Weapon::DeactiveBullet()
{
	activeAmmo--;
}

void Weapon::ActiveBullet()
{
	activeAmmo++;
}

bool Weapon::CanFire()
{
	return false;
}

void Weapon::SetZeroAmmoCallback(const std::function<void()>& callback)
{
	zeroAmmoCallback = callback;
}

D3DXVECTOR2 Weapon::WeaponPos()
{
	auto pos = this->NormalizedPosition();
	float radian = Math::ToRadian(GetAngle());

	pos.x += cosf(radian)*offset;
	pos.y += sinf(radian)*offset;

	return pos;
}

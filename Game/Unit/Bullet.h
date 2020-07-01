#ifndef __BULLET_H__
#define __BULLET_H__

#include "Object/PhysicsObject.h"

class IBulletManager
{
public:
	virtual float GetAngle() = 0;
	virtual void ActiveBullet() = 0;
	virtual void DeactiveBullet() = 0;
};

class Bullet : public PhysicsObject
{
public:
	static Bullet* Create(IBulletManager* manager);
	bool Init(IBulletManager* manager);
	void RemoveManager();
protected:
	
	D3DXVECTOR2 startPosition;

	float moveSpeed;

	bool active;
	IBulletManager* bulletManager;
	float damage;
public:
	Bullet(IBulletManager* manager);
	virtual ~Bullet();

	void SetRunning(bool val);

	void ResetPosition(float angle);
	void SetMoveSpeed(float val) { moveSpeed = val; }
	void SetDamage(float val) { damage = val; }

	void Update();


	bool GetActive() { return active; }
	virtual void SetRotation(float angle);
	virtual void Activate();
	void CreateEffect();
	void DeActive();

	// PhysicsObject을(를) 통해 상속됨
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;

	// PhysicsObject을(를) 통해 상속됨
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;

	// PhysicsObject을(를) 통해 상속됨
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
};

#endif
#pragma once

#include "Game/Unit/MetalUnit.h"
#include "Game/Item/Item.h"

class Slave : public MetalUnit
{
public:
	static Slave* Create(Item* item);
	bool Init(Item* item);

	~Slave();

	void Update() override;



	// PhysicsObject을(를) 통해 상속됨
	virtual void EnterPhysicsCollision(PhysicsObject * b, ColDirection direction) override;
	// PhysicsObject을(를) 통해 상속됨
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;
	// PhysicsObject을(를) 통해 상속됨
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
private:
	bool bFree;
	int phase;
	Item* item;
	Animation* slaveAnim;

	float moveSpeed;
	float runSpeed;
	float distance;

	float turnTimer;

	// IDamageTaken을(를) 통해 상속됨
	virtual void DamageTaken(float damage);

	// MetalUnit을(를) 통해 상속됨
	virtual void Died() override;
};
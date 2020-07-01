#pragma once

#include "Object/PhysicsObject.h"

class Item : public PhysicsObject
{
public:
	bool Init();
	// PhysicsObject을(를) 통해 상속됨
	virtual void EnterPhysicsCollision(PhysicsObject * b, ColDirection direction) override;
	virtual void ItemEffect(PhysicsObject * b) = 0;

	// PhysicsObject을(를) 통해 상속됨
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;

	// PhysicsObject을(를) 통해 상속됨
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
};
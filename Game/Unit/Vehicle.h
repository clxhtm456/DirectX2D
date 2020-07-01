#pragma once

#include "Object/PhysicsObject.h"

class Vehicle : public PhysicsObject
{
	// PhysicsObject을(를) 통해 상속됨
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;

	// PhysicsObject을(를) 통해 상속됨
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;

	// PhysicsObject을(를) 통해 상속됨
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
};
#pragma once

#include "Game/Unit/MetalUnit.h"
#include "Game/Item/WallBlock.h"

class Truck : public MetalUnit
{
public:
	static Truck* Create();
	bool Init();
	~Truck();
	void Update();

	// MetalUnit을(를) 통해 상속됨
	virtual void DamageTaken(float damage) override;
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void ExitPhysicsCollision(PhysicsObject* b) override;
private:
	WallBlock* rideObject;
	Clip* truckClip;

	// MetalUnit을(를) 통해 상속됨
	virtual void Died() override;
};
#pragma once

#include "Game/Unit/MetalUnit.h"

class BossTrain : public MetalUnit
{
public:
	static BossTrain* Create();
	bool Init();

	void Update();

	// MetalUnit을(를) 통해 상속됨
	virtual void DamageTaken(float damage) override;
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void ExitPhysicsCollision(PhysicsObject* b) override;
	virtual void Died() override;
};

class DriveTrain : public MetalUnit
{
public:
	static DriveTrain* Create();
	bool Init();

	void Update();
	// MetalUnit을(를) 통해 상속됨
	virtual void DamageTaken(float damage) override;
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void ExitPhysicsCollision(PhysicsObject* b) override;
	virtual void Died() override;
private:
	Animation* body;
	Animation* frontLeg;
	Animation* behindLeg;
};

class MetalTrain : public MetalUnit
{
public:
	static MetalTrain* Create();
	bool Init();

	void Update();
	// MetalUnit을(를) 통해 상속됨
	virtual void DamageTaken(float damage) override;
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void ExitPhysicsCollision(PhysicsObject* b) override;
	virtual void Died() override;
private:
	Animation* body;
	Animation* frontLeg;
	Animation* behindLeg;
};

class WoodenTrain : public MetalUnit
{
public:
	static WoodenTrain* Create();
	bool Init();

	void Update();
	void Render();
	// MetalUnit을(를) 통해 상속됨
	virtual void DamageTaken(float damage) override;
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
	virtual void ExitPhysicsCollision(PhysicsObject* b) override;
	virtual void Died() override;
private:
	Animation* body;
	Animation* frontLeg;
	Animation* behindLeg;
};
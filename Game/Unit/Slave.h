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



	// PhysicsObject��(��) ���� ��ӵ�
	virtual void EnterPhysicsCollision(PhysicsObject * b, ColDirection direction) override;
	// PhysicsObject��(��) ���� ��ӵ�
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;
	// PhysicsObject��(��) ���� ��ӵ�
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

	// IDamageTaken��(��) ���� ��ӵ�
	virtual void DamageTaken(float damage);

	// MetalUnit��(��) ���� ��ӵ�
	virtual void Died() override;
};
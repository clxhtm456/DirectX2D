#pragma once

#include "Object/PhysicsObject.h"

class Item : public PhysicsObject
{
public:
	bool Init();
	// PhysicsObject��(��) ���� ��ӵ�
	virtual void EnterPhysicsCollision(PhysicsObject * b, ColDirection direction) override;
	virtual void ItemEffect(PhysicsObject * b) = 0;

	// PhysicsObject��(��) ���� ��ӵ�
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;

	// PhysicsObject��(��) ���� ��ӵ�
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
};
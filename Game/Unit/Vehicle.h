#pragma once

#include "Object/PhysicsObject.h"

class Vehicle : public PhysicsObject
{
	// PhysicsObject��(��) ���� ��ӵ�
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;

	// PhysicsObject��(��) ���� ��ӵ�
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;

	// PhysicsObject��(��) ���� ��ӵ�
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
};
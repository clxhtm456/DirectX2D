#pragma once

#include "Game/Unit/MetalUnit.h"
#include "Game/FlagList.h"

class Soldier : public MetalUnit
{
public:
	static Soldier* Create();
	bool Init();

	void Update();

	void DetectPlayer(PhysicsObject* object, ColDirection direction);

	void Attack();

	// MetalUnit��(��) ���� ��ӵ�
	virtual void DamageTaken(float damage) override;
	virtual void EnterPhysicsCollision(PhysicsObject * b, ColDirection direction) override;
	virtual void OnPhysicsCollision(PhysicsObject * b, ColDirection direction) override;
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;
	virtual void Died() override;
private:
	Animation * soldierAnimation;
	EventBox* detectRadius;
	PhysicsObject* _target;
	class Weapon* _weapon;

	float moveSpeed;
	float direction;

	float turnTimer;

	float attackCoolTime;

	int phase;
	/*
	phase 0 : �ι� Ÿ���� ������� ���� Ÿ���� �߽߰� ��� ���� ������ 1 ����
	phase 1 : ������ ������ Ÿ�ٿ��� �����Ÿ� �ٰ����� ������ ���� �� �ѹ��� �ٰ����� �Ÿ��� �ִ�Ÿ��� ����������
	*/
};
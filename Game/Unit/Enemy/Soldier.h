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

	// MetalUnit을(를) 통해 상속됨
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
	phase 0 : 로밍 타겟이 없을경우 시행 타겟을 발견시 놀람 이후 페이즈 1 돌입
	phase 1 : 전투중 페이즈 타겟에게 일정거리 다가간후 공격을 시행 단 한번에 다가가는 거리는 최대거리가 정해져있음
	*/
};
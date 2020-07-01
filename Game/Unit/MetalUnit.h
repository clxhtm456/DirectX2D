#pragma once

#include "Game/Unit/IDamageTaken.h"

#define TAG_ENEMY	2

class MetalUnit : public PhysicsObject, public IDamageTaken
{
public:
	// PhysicsObject을(를) 통해 상속됨
	virtual void EnterPhysicsCollision(PhysicsObject * b, ColDirection direction) override = 0;
	virtual void OnPhysicsCollision(PhysicsObject * b, ColDirection direction) override = 0;
	virtual void ExitPhysicsCollision(PhysicsObject * b) override = 0;
	void SetDiedCallback(const std::function<void()>& func) { diedCallback = func; }
	virtual void Died() = 0;

	void SetHpPoint(float value)
	{
		hpPoint = value;
		if (hpPoint <= 0)
		{
			if(diedCallback != nullptr)
				diedCallback();
			Died();
		}
			
	}
	void StartTwinkle(Animation* animation)
	{
		twinkleCount = 5;
		bTwinkle = true;
		Twinkle(animation);
	}

	float GetHpPoint()
	{
		return hpPoint;
	}

protected:
	float hpPoint;
	int twinkleCount;
	bool bTwinkle;
	std::function<void()> diedCallback = nullptr;
private:
	void Twinkle(Animation* animation);
};
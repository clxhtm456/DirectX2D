#pragma once

class EventBox : public PhysicsObject
{
public:
	static EventBox* Create(const std::function<void(PhysicsObject*, ColDirection)>& func,float width,float height);
	bool Init(const std::function<void(PhysicsObject*, ColDirection)>& func, float width, float height);

	void OnceEvent(bool val) { _onceEvent = val; }

	void SetWidth(float val);
	void SetHeight(float val);
private:
	EventBox();
	~EventBox();

	bool _onceEvent;

	std::function<void(PhysicsObject*, ColDirection)> eventCallback;

	// PhysicsObject을(를) 통해 상속됨
	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) override;

	// PhysicsObject을(를) 통해 상속됨
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;

	// PhysicsObject을(를) 통해 상속됨
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;
};
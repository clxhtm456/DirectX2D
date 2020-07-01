#include "stdafx.h"
#include "EventBox.h"

EventBox* EventBox::Create(const std::function<void(PhysicsObject*,ColDirection)>& func, float width, float height)
{
	EventBox* pRet = new EventBox();
	if (pRet && pRet->Init(func,width,height))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool EventBox::Init(const std::function<void(PhysicsObject*, ColDirection)>& func, float width, float height)
{
	if (!PhysicsObject::Init())
		return false;

	AnchorPosition(0.5f, 0.5f);
	collider->Width(width);
	collider->Height(height);

	isKinematic = true;
	eventCallback = func;

	return true;
}

void EventBox::SetWidth(float val)
{
	collider->Width(val);
}

void EventBox::SetHeight(float val)
{
	collider->Height(val);
}

EventBox::EventBox()
{
}

EventBox::~EventBox()
{
}

void EventBox::EnterPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
	if (eventCallback != nullptr)
		eventCallback(b,direction);
	//RemoveSelf();
}

void EventBox::ExitPhysicsCollision(PhysicsObject * b)
{
}

void EventBox::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

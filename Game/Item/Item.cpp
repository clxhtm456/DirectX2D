#include "stdafx.h"
#include "Item.h"
#include "Game/FlagList.h"

bool Item::Init()
{
	if(!PhysicsObject::Init())
		return false;

	AddFlag(FLAG_ITEM);

	return true;
}

void Item::EnterPhysicsCollision(PhysicsObject * b, ColDirection direction)
{
	ItemEffect(b);
}

void Item::ExitPhysicsCollision(PhysicsObject * b)
{
}

void Item::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
}

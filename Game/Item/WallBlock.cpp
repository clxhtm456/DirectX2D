#include "stdafx.h"
#include "WallBlock.h"

WallBlock * WallBlock::Create()
{
	WallBlock* pRet = new WallBlock();
	if (pRet && pRet->Init())
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

bool WallBlock::Init()
{
	if (!__super::Init())
		return false;

	topcol =	true;
	bottomcol = true;
	rightcol =	true;
	leftcol =	true;

	recentPos = D3DXVECTOR2(0, 0);

	return true;
}

WallBlock::~WallBlock()
{
	for (auto col : collisionList)
	{
		//ExitCollisionAll(col);
		ExitPhysicsCollision(col);
	}
}

void WallBlock::Update()
{
	D3DXVECTOR2 delta = NormalizedPosition() - recentPos;
	recentPos = NormalizedPosition();

	for (auto iter = directionList.begin(); iter != directionList.end(); iter++)
	{
		if ((*iter).first->GetKinematic() == true)
			continue;
		switch ((*iter).second)
		{
		case UP:
		{
			auto position = (*iter).first->Position();
			(*iter).first->Position(position+delta);
		}
			break;
		case RIGHT:
		{
			if (delta.x > 0)
			{
				auto position = (*iter).first->Position();
				(*iter).first->Position(position.x + delta.x,position.y);
			}
		}
			break;
		case LEFT:
		{
			if (delta.x < 0)
			{
				auto position = (*iter).first->Position();
				(*iter).first->Position(position.x + delta.x, position.y);
			}
		}
			break;
		}
	}
	PhysicsObject::Update();
}

void WallBlock::EnterPhysicsCollision(PhysicsObject * b, ColDirection direction)
{
	/*b->ObjBottomCollision(false);
	b->ObjTopCollision(false);
	b->ObjLeftCollision(false);
	b->ObjRightCollision(false);*/

	switch (direction)
	{
	case UP:
		if(topcol)
			b->ObjBottomCollision(true);
		break;
	case DOWN:
		if(bottomcol)
			b->ObjTopCollision(true);
		break;
	case RIGHT:
		if(rightcol)
			b->ObjLeftCollision(true);
		break;
	case LEFT:
		if(leftcol)
			b->ObjRightCollision(true);
		break;
	default:
		break;
	}

	directionList.push_back(pair<PhysicsObject *, ColDirection>(b, direction));
	
}

void WallBlock::ExitPhysicsCollision(PhysicsObject * b)
{
	for (auto iter = directionList.begin(); iter != directionList.end(); iter++)
	{
		if ((*iter).first == b)
		{
			switch ((*iter).second)
			{
			case UP:
				b->ObjBottomCollision(false);
				break;
			case DOWN:
				b->ObjTopCollision(false);
				break;
			case RIGHT:
				b->ObjLeftCollision(false);
				break;
			case LEFT:
				b->ObjRightCollision(false);
				break;
			}
			iter = directionList.erase(iter);
			break;
		}
	}
	
}

void WallBlock::OnPhysicsCollision(PhysicsObject* b, ColDirection direction)
{
	/*b->ObjBottomCollision(false);
	b->ObjTopCollision(false);
	b->ObjLeftCollision(false);
	b->ObjRightCollision(false);
	switch (direction)
	{
	case UP:
		if (topcol)
			b->ObjBottomCollision(true);
		break;
	case DOWN:
		if (bottomcol)
			b->ObjTopCollision(true);
		break;
	case RIGHT:
		if (rightcol)
			b->ObjLeftCollision(true);
		break;
	case LEFT:
		if (leftcol)
			b->ObjRightCollision(true);
		break;
	default:
		break;
	}*/
}

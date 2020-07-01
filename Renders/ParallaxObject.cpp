#include "stdafx.h"
#include "ParallaxObject.h"

ParallaxObject * ParallaxObject::Create(Camera* camera, float parallaxFactorX, float parallaxFactorY)
{
	ParallaxObject* pRet = new ParallaxObject();
	if (pRet && pRet->Init(camera, parallaxFactorX,parallaxFactorY))
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

bool ParallaxObject::Init(Camera* camera,float parallaxFactorX,float parallaxFactorY)
{
	if(!Node::Init())
		return false;

	camera->AddMoveCallback(CC_CALLBACK_2(ParallaxObject::Move, this));
	this->parallaxFactorX = parallaxFactorX;
	this->parallaxFactorY = parallaxFactorY;

	return true;
}

ParallaxObject::~ParallaxObject()
{
}

void ParallaxObject::Move(float deltaX, float deltaY)
{
	D3DXVECTOR2 newPos = Position();
	newPos.x -= deltaX * parallaxFactorX;
	newPos.y -= deltaY * parallaxFactorY;

	Position(newPos);
}

void ParallaxObject::SetParallaxFactorX(float val)
{
	parallaxFactorX = val;
}
void ParallaxObject::SetParallaxFactorY(float val)
{
	parallaxFactorY = val;
}
#include "stdafx.h"
#include "Animation.h"

Animation* Animation::Create()
{
	Animation* pRet = new Animation();
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

bool Animation::Init()
{
	if (!Node::Init())
		return false;
	return true;
}

Animation::Animation():
	currentClip(-1)
{
}

Animation::~Animation()
{
}

void Animation::AddClip(Clip * clip)
{
	clips.push_back(clip);
	AddChild(clip);
	//clip->AnchorPosition(_anchorPoint);
	//clip->retain();
}

void Animation::Play(int clipNumber)
{
	if (clipNumber == currentClip && clips[currentClip]->GetMode() == PlayMode::Loop)
		return;

	if (clipNumber > -1 && currentClip > -1)
		clips[currentClip]->Stop();

	currentClip = clipNumber;
	clips[currentClip]->Play();
}

void Animation::Stop()
{
	if (currentClip > -1)
		clips[currentClip]->Stop();
}


D3DXVECTOR2 Animation::TextureSize()
{
	if (currentClip < 0)
		return D3DXVECTOR2(0, 0);

	return clips[currentClip]->TextureSize();
}

void Animation::Update()
{
	if (currentClip < 0)
		return;

	clips[currentClip]->Update();
}
void Animation::Render()
{
	if (_visible == false)
		return;
	if (currentClip < 0)
		return;

	clips[currentClip]->Render();
}

void Animation::DrawBound(bool val)
{
	for (auto clip : clips)
	{
		clip->DrawBound(val);
	}
}

Clip * Animation::GetClip()
{
	if (currentClip < 0)
		return nullptr;

	return clips[currentClip];
}

Sprite * Animation::GetSprite()
{
	return clips[currentClip]->GetSprite();
}

void Animation::DrawBloom(bool val, float r, float g, float b)
{
	DrawBloom(val, D3DXCOLOR(r, g, b, 0));
}

void Animation::DrawBloom(bool val,D3DXCOLOR color)
{
	for (auto clip : clips)
		clip->DrawBloom(val, color);
}

void Animation::DrawTwinkle(bool val)
{
	for (auto clip : clips)
		clip->DrawTwinkle(val);
}

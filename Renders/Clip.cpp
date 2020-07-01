#include "stdafx.h"
#include "Clip.h"

//---------------------------------------------------------------------------------------
// Frame(Struct)
//---------------------------------------------------------------------------------------
Frame::Frame(Sprite * sprite, float v_time)
{
	image = sprite; //프레임 이미지 한장
	time = v_time;	//프레임 한장을 띄우는 시간
}

Frame::~Frame()
{
}

//---------------------------------------------------------------------------------------
// Clip(Class)
//---------------------------------------------------------------------------------------

Clip::Clip():
	length(0), 
	playTime(0),
	bPlay(false),
	currentFrame(0),
	startCallback(NULL), endCallback(NULL)
{
}

Clip* Clip::Create(PlayMode mode, float speed)
{
	Clip* pRet = new Clip();
	if (pRet && pRet->Init(mode,speed))
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

bool Clip::Init(PlayMode mode, float speed)
{
	if(!Node::Init())
		return false;

	this->mode = mode;
	this->speed = speed;

	return true;
}

Clip::~Clip()
{
	for (Frame* frame : frames)
		SAFE_DELETE(frame);
}

void Clip::AddFrame(Sprite * sprite, float time)
{
	frames.push_back(new Frame(sprite, time));
	AddChild(sprite);
	sprite->AnchorPosition(_anchorPoint);
}

void Clip::AddFrame(const function<void()>& func)
{
	auto callback = make_pair(frames.size(),func);
	callbackList.push_back(callback);
}

void Clip::SetStartCallBack(const function<void()>& func)
{
	startCallback = func;
}

void Clip::SetEndCallBack(const function<void()>& func)
{
	endCallback = func;
}

D3DXVECTOR2 Clip::TextureSize()
{
	return frames[currentFrame]->image->TextureSize();
}

void Clip::Play()
{
	if (startCallback != NULL)
		startCallback();
	playTime = 0.0f;
	currentFrame = 0;
	bPlay = true;
}

void Clip::Play(UINT startFrame)
{
	playTime = 0.0f;
	currentFrame = startFrame;
	bPlay = true;
}

void Clip::Stop()
{
	if (!bPlay)
		return;
	bPlay = false;
	currentFrame = frames.size()-1;
	if (endCallback != NULL)
		endCallback();
}

void Clip::Update()
{
	Frame* frame = frames[currentFrame];

	if (bPlay == true)
	{
		float time = frame->time  * speed; //time : 그림 한장을 띄우는 시간
		playTime += Time::deltaTime(); //리얼 타이머

		if (playTime >= time)
		{
			switch (mode)
			{
			case PlayMode::End:
			{
				currentFrame++;
				for (auto callback : callbackList)
				{
					if (callback.first == currentFrame)
					{
						if (callback.second != NULL)
							callback.second();
					}
				}
				
				if (currentFrame >= frames.size())
				{
					Stop();
				}
			}
			break;
			case PlayMode::Loop:
			{
				
				currentFrame++;
				for (auto callback : callbackList)
				{
					if (callback.first == currentFrame)
					{
						if (callback.second != NULL)
							callback.second();
					}
				}
				currentFrame %= frames.size();
			}
			break;
			case PlayMode::Reverse:
				break;
			}

			playTime = 0.0f;
		}
	}

	frames[currentFrame]->image->Update();
}

void Clip::Render()
{
	if (_visible == false)
		return;

	Frame* frame = frames[currentFrame];
	frame->image->Render();
}

void Clip::AnchorPosition(float x, float y)
{
	Node::AnchorPosition(x, y);
	for (Frame* frame : frames)
		frame->image->AnchorPosition(x,y);
}

void Clip::AnchorPosition(D3DXVECTOR2 vec)
{
	Node::AnchorPosition(vec);
	for (Frame* frame : frames)
		frame->image->AnchorPosition(vec);
}

void Clip::DrawBound(bool val)
{
	for (Frame* frame : frames)
		frame->image->DrawBound(val);
}

void Clip::DrawBloom(bool val, float r, float g, float b)
{
	DrawBloom(val, D3DXCOLOR(r, g, b, 0));
}

void Clip::DrawBloom(bool val, D3DXCOLOR color)
{
	for (Frame* frame : frames)
		frame->image->DrawBloom(val, color);
}

void Clip::DrawTwinkle(bool val)
{
	for (Frame* frame : frames)
		frame->image->DrawTwinkle(val);
}

Sprite * Clip::GetSprite()
{
	return frames[currentFrame]->image;
}

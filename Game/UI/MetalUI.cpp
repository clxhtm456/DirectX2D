#include "stdafx.h"
#include "MetalUI.h"

MetalUI* MetalUI::Create()
{
	MetalUI* pRet = new MetalUI();
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

bool MetalUI::Init()
{
	if (!Node::Init())
		return false;

	auto uiScore = Sprite::Create(Textures + L"MetalSlug/Score/UI.png");
	//uiScore->Scale(1.5f, 1.5f);
	uiScore->AnchorPosition(0,1);

	auto number = Node::Create();
	D3DXVECTOR2 offset = D3DXVECTOR2(-10,-4);
	number->Scale(0.8f, 0.8f);
	number->Position(uiScore->TextureSize().x*0.5f+offset.x, uiScore->TextureSize().y * -0.5f+offset.y);
	uiScore->AddChild(number);
	CreateCenterNumber(number);
	
	AddChild(uiScore);

	lastCount = -1;

	return true;
}

MetalUI::~MetalUI()
{
}

void MetalUI::CreateCenterNumber(Node* center)
{

	for (int i = 0; i < 3; i++)
	{
		for (int z = 0; z < 10; z++)
		{
			sprites[i].push_back(Sprite::Create(Textures + L"MetalSlug/Score/" + (to_wstring(z) + L".png")));
			sprites[i][z]->SetVisible(false);
			sprites[i][z]->Position(-8 + i * 8, 0);
			center->AddChild(sprites[i][z]);
		}
	}
	sprites[0].push_back(Sprite::Create(Textures + L"MetalSlug/Score/mu.png"));
	sprites[1].push_back(Sprite::Create(Textures + L"MetalSlug/Score/han.png"));
	sprites[2].push_back(Sprite::Create(Textures + L"MetalSlug/Score/dae.png"));
	//mu->SetVisible(false);
	//han->SetVisible(false);
	//->SetVisible(false);
	sprites[0][10]->Position(-8, 0);
	sprites[2][10]->Position(8, 0);
	center->AddChild(sprites[0][10]);
	center->AddChild(sprites[1][10]);
	center->AddChild(sprites[2][10]);
	

}

void MetalUI::InvisibleNumber()
{
	if (lastCount == -1)
	{
		sprites[0][10]->SetVisible(false);
		sprites[1][10]->SetVisible(false);
		sprites[2][10]->SetVisible(false);
	}
	else
	{
		sprites[0][Math::NumberOfFigure(lastCount,3)]->SetVisible(false);
		sprites[1][Math::NumberOfFigure(lastCount, 2)]->SetVisible(false);
		sprites[2][Math::NumberOfFigure(lastCount, 1)]->SetVisible(false);
	}
}

void MetalUI::VisibleNumber(int number)
{
	if (number == -1)
	{
		sprites[0][10]->SetVisible(true);
		sprites[1][10]->SetVisible(true);
		sprites[2][10]->SetVisible(true);
	}
	else
	{
		sprites[0][Math::NumberOfFigure(number, 3)]->SetVisible(true);
		sprites[1][Math::NumberOfFigure(number, 2)]->SetVisible(true);
		sprites[2][Math::NumberOfFigure(number, 1)]->SetVisible(true);
	}
	lastCount = number;
}

int MetalUI::AmmoCalc(int val)
{
	InvisibleNumber();
	VisibleNumber(val);
	return 0;
}

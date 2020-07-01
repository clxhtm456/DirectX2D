#include "stdafx.h"
#include "Stage.h"

#include "Viewer/ParallaxCamera.h"
#include "Renders/ParallaxObject.h"
#include "Renders/Line.h"

#include "Game/Unit/Player.h"
#include "Game/Unit/Slave.h"
#include "Game/Item/Item_H.h"
#include "Game/Item/WallBlock.h"
#include "Game/UI/MetalUI.h"

#include "Game/Unit/Enemy/Truck.h"
#include "Game/Unit/Enemy/Soldier.h"
#include "Game/Unit/Boss/Train.h"


#include "Scene/FileManager.h"
#include "Viewer/FreeCamera.h"

#include "Game/FlagList.h"
static float xvalue = 1.0f;
static float yvalue = 1.0f;




////---------------
/*
Flag01 Player
Flag02 Enemy
*/
////---------------
Stage * Stage::Create(wstring mapResource)
{
	Stage* pRet = new Stage();
	if (pRet && pRet->Init(mapResource))
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

bool Stage::Init(wstring mapResource)
{
	if(!Scene::Init())
		return false;

	player = Player::Create();
	player->Position(200, 300);
	player->Scale(2.0f,2.0f);
	AddChild(player,5);


	_defaultCamera->removeFromParent();
	_defaultCamera = FollowCamera::Create();
	_defaultCamera->SetMgnfc(1.5f);
	dynamic_cast<FollowCamera*>(_defaultCamera)->SetLockLeftPoint(true);
	dynamic_cast<FollowCamera*>(_defaultCamera)->SetLimitLeftPoint(0.0f);
	dynamic_cast<FollowCamera*>(_defaultCamera)->SetLimitLeftPoint(true);
	dynamic_cast<FollowCamera*>(_defaultCamera)->SetLimitRightPoint(2100.0f);
	dynamic_cast<FollowCamera*>(_defaultCamera)->SetLimitRightPoint(true);
	dynamic_cast<FollowCamera*>(_defaultCamera)->ChangeTarget(player);
	AddChild(_defaultCamera);

	auto eventBox = WallBlock::Create();
	eventBox->AnchorPosition(0, 0);
	eventBox->SetKinematic(true);
	eventBox->Position(2506, 300);
	eventBox->GetCollider()->Width(192);
	eventBox->GetCollider()->Height(60);
	eventBox->BottomCol(false);
	eventBox->AddFlag(FLAG_AL);
	AddChild(eventBox, 4);

	eventBox = WallBlock::Create();
	eventBox->AnchorPosition(0, 0);
	eventBox->SetKinematic(true);
	eventBox->Position(2557, 363);
	eventBox->GetCollider()->Width(100);
	eventBox->GetCollider()->Height(50);
	eventBox->BottomCol(false);
	eventBox->AddFlag(FLAG_AL);
	AddChild(eventBox, 4);

	eventBox = WallBlock::Create();
	eventBox->AnchorPosition(0, 0);
	eventBox->SetKinematic(true);
	eventBox->Position(2700, 530);
	eventBox->GetCollider()->Width(200);
	eventBox->GetCollider()->Height(20);
	eventBox->BottomCol(false);
	eventBox->LeftCol(false);
	eventBox->RightCol(false);
	eventBox->AddFlag(FLAG_AL);
	AddChild(eventBox, 4);

	auto wave1Box = EventBox::Create(CC_CALLBACK_0(Stage::Wave1Start, this), 50, 500);
	wave1Box->Flag(FLAG_EVENT);
	wave1Box->Position(1410, 400);
	AddChild(wave1Box,5);

	auto soldier = Soldier::Create();
	soldier->Position(800, 300);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(600, 300);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	auto slave = Slave::Create(Item_H::Create());
	slave->Position(600, 300);
	slave->Scale(2.0f, 2.0f);
	AddChild(slave, 4);

	/*auto truck = Truck::Create();
	truck->Position(800, 400);
	truck->Scale(2.0f, 2.0f);
	AddChild(truck, 6);*/

	/*auto soldier = Soldier::Create();
	soldier->Position(800, 400);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);*/

	/*auto train = WoodenTrain::Create();
	train->Position(800, 600);
	train->Scale(2.0f, 2.0f);
	AddChild(train, 4);*/


	/*auto itemH = Item_H::Create();
	itemH->Position(600, 300);
	itemH->AddFlag(FLAG_01);
	AddChild(itemH, 4);*/

	



	DrawBackGround();
	DrawUI();

	//Map Loading
	{
		FileManager::Load(mapResource);

		//Line
		auto linePoints = FileManager::GetLineData();
		auto lineSize = linePoints.size();
		for (size_t i = 0; i < lineSize; i++)
		{
			lines.push_back(new Line(linePoints[i].first, linePoints[i].second));
		}


		////Object
		//auto objectPos = FileManager::GetObjectData();

		//auto objectSize = objectPos.size();
		//for (int i = 0; i < objectSize; i++)
		//{
		//	ObjectCreate(values->m_MainCamera,objectPos[i].first, objectPos[i].second);
		//}
	}

	return true;
}

void Stage::DrawBackGround()
{
	wstring backGroundTexture = Textures + L"MetalSlug/BackGround.png";
	wstring skyTexture = Textures + L"MegaMan/Scene/BackGround/mmzxa_asheintro.png";

	auto bgSky = ParallaxObject::Create(_defaultCamera, 0.05f);
	bgSky->Position(-20, 328);
	AddChild(bgSky, 1);

	auto skySprite = Sprite::Create(backGroundTexture, 71, 43, 8518, 553);
	skySprite->AnchorPosition(0, 0);
	bgSky->AddChild(skySprite);

	auto grassSprite1 = Clip::Create(PlayMode::Loop);
	grassSprite1->AnchorPosition(0, 0);
	for (int i = 0; i < 8; i++)
		grassSprite1->AddFrame(Sprite::Create(backGroundTexture, 14, 1275 + (i * 256), 2061, 1531 + (i * 256)), 0.1f);
	grassSprite1->Position(0, 94);
	grassSprite1->Play();
	AddChild(grassSprite1, 7);

	auto grassSprite2 = Clip::Create(PlayMode::Loop);
	grassSprite2->AnchorPosition(0, 0);
	grassSprite2->Position(0, 256);
	grassSprite2->AddFrame(Sprite::Create(backGroundTexture, 2128, 2060 - 257, 4078, 2060), 0.1f);
	grassSprite2->AddFrame(Sprite::Create(backGroundTexture, 2128, 2318 - 257, 4078, 2318), 0.1f);
	grassSprite2->AddFrame(Sprite::Create(backGroundTexture, 2128, 2576 - 257, 4078, 2576), 0.1f);
	grassSprite2->AddFrame(Sprite::Create(backGroundTexture, 2128, 2834 - 257, 4078, 2834), 0.1f);
	grassSprite2->AddFrame(Sprite::Create(backGroundTexture, 2128, 3092 - 257, 4078, 3092), 0.1f);
	grassSprite2->AddFrame(Sprite::Create(backGroundTexture, 2128, 3350 - 257, 4078, 3350), 0.1f);
	grassSprite2->AddFrame(Sprite::Create(backGroundTexture, 2128, 3608 - 257, 4078, 3608), 0.1f);
	grassSprite2->AddFrame(Sprite::Create(backGroundTexture, 2128, 3866 - 257, 4078, 3866), 0.1f);
	grassSprite2->Play();
	AddChild(grassSprite2, 3);

	auto riverSprite = Clip::Create(PlayMode::Loop);
	riverSprite->AnchorPosition(0, 0);
	for (int i = 0; i < 8; i++)
		riverSprite->AddFrame(Sprite::Create(backGroundTexture, 4711, 1309 + (i * 256), 6840, 1565 + (i * 256)), 0.1f);
	riverSprite->Position(4696, 31.6);
	riverSprite->Play();
	AddChild(riverSprite,3);


	auto towerSprite = Sprite::Create(backGroundTexture, 2094, 1263, 3933, 1749);
	towerSprite->AnchorPosition(0, 0);
	towerSprite->Position(2001, 151.23);
	AddChild(towerSprite, 7);

	auto floorSprite = Sprite::Create(backGroundTexture, 75, 558, 8778, 1284);
	floorSprite->AnchorPosition(0, 0);
	AddChild(floorSprite, 2);

}

void Stage::DrawUI()
{
	auto uiObject = ParallaxObject::Create(_defaultCamera, 1.0f,1.0f);
	uiObject->Position(Width*-0.5f*(float)(1/_defaultCamera->GetMgnfc()), Height * 0.5f * (float)(1 / _defaultCamera->GetMgnfc()));
	AddChild(uiObject, 10);

	auto hpScore = Sprite::Create(Textures + L"MetalSlug/Score/HP.png");
	hpScore->AnchorPosition(0, 1);
	hpScore->Position(20, -20);
	hpScore->Scale(2.0f, 2.0f);
	uiObject->AddChild(hpScore);

	ammoUI = MetalUI::Create();
	ammoUI->AnchorPosition(0, 1);
	ammoUI->Position(150, -30);
	ammoUI->Scale(3.0f, 3.0f);
	uiObject->AddChild(ammoUI);

	player->SetAmmoObserver(ammoUI);


	auto camObject = ParallaxObject::Create(_defaultCamera, 1.0f, 1.0f);
	camObject->Position(0, 0);
	AddChild(camObject, 4);

	auto camBlock = WallBlock::Create();
	camBlock->AnchorPosition(1.0f, 0.5f);
	camBlock->SetKinematic(true);
	camBlock->Position(Width * -0.5f * (float)(1 / _defaultCamera->GetMgnfc()), 0);
	camBlock->GetCollider()->Width(50);
	camBlock->GetCollider()->Height(400);
	camBlock->BottomCol(false);
	camBlock->AddFlag(FLAG_CAMERAOB);
	camObject->AddChild(camBlock);

	camBlock = WallBlock::Create();
	camBlock->AnchorPosition(0.0f, 0.5f);
	camBlock->SetKinematic(true);
	camBlock->Position(Width * +0.5f * (float)(1 / _defaultCamera->GetMgnfc()), 0);
	camBlock->GetCollider()->Width(50);
	camBlock->GetCollider()->Height(400);
	camBlock->BottomCol(false);
	camBlock->AddFlag(FLAG_CAMERAOB);
	camObject->AddChild(camBlock);
}

Stage::Stage() :
	bLineVisible(false),
	slope(0.0f)
{
}

Stage::~Stage()
{
	for (auto line : lines)
		delete line;
}

void Stage::Update()
{
	Scene::Update();
}

void Stage::Render()
{
	Scene::Render();
}

void Stage::TestFunction()
{
		auto bullet = Sprite::Create(Textures+L"MetalSlug/shot.png");
		AddChild(bullet);
}

void Stage::ObjectCreate(Camera* cam,UINT type,D3DXVECTOR2 vec)
{
	//switch ((Enemy_Entry)type)
	//{
	//case Enemy_Entry::Arabian:
	//{
	//	//auto arabian = new Arabian(cam,vec, D3DXVECTOR2(4, 4));
	//	break;
	//}
	//default:
	//	break;
	//}
}

void Stage::Wave1Start()
{
	if (phase != 0)
		return;
	phase = 1;
	auto slave = Slave::Create(Item_H::Create());
	slave->Position(2110, 400);
	slave->Scale(2.0f, 2.0f);
	AddChild(slave, 4);

	auto truck = Truck::Create();
	truck->Position(2060, 350);
	truck->Scale(2.0f, 2.0f);
	truck->SetDiedCallback(CC_CALLBACK_0(Stage::Wave1Clear, this));
	AddChild(truck, 6);

	auto soldier = Soldier::Create();
	soldier->Position(2110, 400);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(2050, 400);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);
}

void Stage::Wave1Clear()
{
	phase = 2;

	auto soldier = Soldier::Create();
	soldier->Position(2800, 600);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(2800, 360);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(3000, 360);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(3400, 360);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(3600, 360);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(5700, 255);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(7310, 320);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	soldier = Soldier::Create();
	soldier->Position(7200, 320);
	soldier->Scale(2.0f, 2.0f);
	AddChild(soldier, 4);

	dynamic_cast<FollowCamera*>(_defaultCamera)->SetLimitRightPoint(8690.0f);
}

void Stage::Wave2Start()
{
}

void Stage::Wave2Clear()
{
}

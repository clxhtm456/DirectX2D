#include "stdafx.h"
#include "MapEditor.h"
#include "FileManager.h"

#include "Renders/Line.h"
#include "Object/PhysicsObject.h"

#include "Viewer/FreeCamera.h"
#include "Scene.h"



MapEditor * MapEditor::Create(Scene* map)
{
	MapEditor* pRet = new MapEditor();
	if (pRet && pRet->Init(map))
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

bool MapEditor::Init(Scene* map)
{
	if(!Scene::Init())
		return false;

	_scene = map;
	//map->getDefaultCamera()->release();
	//_defaultCamera->removeFromParent();
	//_defaultCamera = map->getDefaultCamera();
	_defaultCamera->SetMgnfc(0.5f);
	linePoints = FileManager::GetLineData();
	//Director::getInstance()->DebugOption(true);

	AddChild(_scene);
	return true;
}

MapEditor::MapEditor() :
	Scene(),
	cursor(0, 0),
	cameraPos(0, 0),
	position(0, 0),
	bDrawLine(false),
	selectItem(-1),
	selectLinePos(0)
{
}

MapEditor::~MapEditor()
{
}

void MapEditor::PlayScene(bool val)
{
	Scene::PlayScene(val);
	Director::getInstance()->DebugOption(val);
}

void MapEditor::Update()
{
	Scene::Update();
	if (Key->Press(VK_LEFT))
	{
		CameraMove(D3DXVECTOR2(-1, 0) * 500.0f);
	}
	else if (Key->Press(VK_RIGHT))
	{
		CameraMove(D3DXVECTOR2(1, 0) * 500.0f);
	}

	if (Key->Press(VK_UP))
	{
		CameraMove(D3DXVECTOR2(0, 1) * 500.0f);
	}
	else if (Key->Press(VK_DOWN))
	{
		CameraMove(D3DXVECTOR2(0, -1) * 500.0f);
	}


	if (bDrawLine)
	{
		if (Key->Press(VK_SHIFT))
		{
			position.y = lastPosition.y;
		}
		auto line = _scene->lines.back();
		line->RightPoint(position);
		linePoints.back().second = position;
	}

	if (selectItem != -1)
	{
		if (selectLinePos == 0)
		{
			_scene->lines.at(selectItem)->LeftPoint(position);
			linePoints.at(selectItem).first = position;
		}
		else
		{
			_scene->lines.at(selectItem)->RightPoint(position);
			linePoints.at(selectItem).second = position;
		}
	}


	/*for (auto object : objectList)
		object->Update();*/

	//마우스 위치 구하기
	cursor = Mouse->Position();
	cursor.x = cursor.x - (float)Width *0.5f;
	cursor.y = (cursor.y - (float)Height*0.5f)*-1.0f;
	cameraPos = Camera::MainCamera()->Position();
	position = (cursor*(1 / Camera::MainCamera()->GetMgnfc()) + cameraPos);

	//backGround->Update();
	
}

void MapEditor::Render()
{
	Scene::Render();
	RenderImGui();
	//RenderDebug();

	//backGround->Render();
	for (auto line : _scene->lines)
		line->Render();
	for (auto object : objectList)
		object->Render();
}

void MapEditor::EditLine()
{
	bool hovering = ImGui::GetIO().WantCaptureMouse;
	if (hovering)
		return;
	if (Mouse->Down(0))
	{
		bDrawLine = true;
		lastPosition = position;
		for (size_t i = 0; i < _scene->lines.size(); i++)
		{
			if (position.x < _scene->lines[i]->LeftPoint().x + 10.0f &&
				position.x > _scene->lines[i]->LeftPoint().x - 10.0f &&
				position.y < _scene->lines[i]->LeftPoint().y + 10.0f &&
				position.y > _scene->lines[i]->LeftPoint().y - 10.0f)
			{
				lastPosition = _scene->lines[i]->LeftPoint();
				//_scene->lines[i]->LeftPoint(position);
			}

			if (position.x < _scene->lines[i]->RightPoint().x + 10.0f &&
				position.x > _scene->lines[i]->RightPoint().x - 10.0f &&
				position.y < _scene->lines[i]->RightPoint().y + 10.0f &&
				position.y > _scene->lines[i]->RightPoint().y - 10.0f)
			{
				lastPosition = _scene->lines[i]->RightPoint();
				//_scene->lines[i]->RightPoint(position);
			}
		}
		
		_scene->lines.push_back(new Line(lastPosition, position));
		linePoints.push_back(pair<D3DXVECTOR2, D3DXVECTOR2>(lastPosition, position));
	}
	else if (Mouse->Up(0))
	{
		bDrawLine = false;
		
	}

	if (Mouse->Down(2))
	{
		for (size_t i = 0; i < _scene->lines.size(); i++)
		{
			if (position.x < _scene->lines[i]->LeftPoint().x + 10.0f &&
				position.x > _scene->lines[i]->LeftPoint().x - 10.0f&&
				position.y < _scene->lines[i]->LeftPoint().y + 10.0f&&
				position.y > _scene->lines[i]->LeftPoint().y - 10.0f)
			{
				selectItem = i;
				selectLinePos = 0;
				//_scene->lines[i]->LeftPoint(position);
			}

			if (position.x < _scene->lines[i]->RightPoint().x + 10.0f &&
				position.x > _scene->lines[i]->RightPoint().x - 10.0f&&
				position.y < _scene->lines[i]->RightPoint().y + 10.0f&&
				position.y > _scene->lines[i]->RightPoint().y - 10.0f)
			{
				selectItem = i;
				selectLinePos = 1;
				//_scene->lines[i]->RightPoint(position);
			}
		}
	}else if (Mouse->Up(2) && selectItem != -1)
	{
		if (selectLinePos == 0)
		{
			linePoints[selectItem].first = position;
		}
		else
		{
			linePoints[selectItem].second = position;
		}
		selectItem = -1;
	}

	if (selectItem == -1)
	{
		if (Mouse->Down(1))
		{
			for (size_t i = 0; i < _scene->lines.size(); i++)
			{
				if (position.x < _scene->lines[i]->LeftPoint().x + 10.0f &&
					position.x > _scene->lines[i]->LeftPoint().x - 10.0f&&
					position.y < _scene->lines[i]->LeftPoint().y + 10.0f&&
					position.y > _scene->lines[i]->LeftPoint().y - 10.0f)
				{
					auto temp = _scene->lines.at(i);
					_scene->lines.erase(_scene->lines.begin() + i);
					delete temp;
					linePoints.erase(linePoints.begin() + i);
					break;
				}

				if (position.x < _scene->lines[i]->RightPoint().x + 10.0f &&
					position.x > _scene->lines[i]->RightPoint().x - 10.0f&&
					position.y < _scene->lines[i]->RightPoint().y + 10.0f&&
					position.y > _scene->lines[i]->RightPoint().y - 10.0f)
				{
					auto temp = _scene->lines.at(i);
					_scene->lines.erase(_scene->lines.begin() + i);
					delete temp;
					linePoints.erase(linePoints.begin() + i);
					break;
				}
			}
		}
	}
	
}

void MapEditor::EditArabian()
{
	bool hovering = ImGui::GetIO().WantCaptureMouse;
	if (hovering)
		return;

	/*if (Mouse->Down(0))
	{
		auto arabian = new Arabian(nullptr,position, D3DXVECTOR2(4, 4));
		arabian->PhysicsActive(false);
		objectList.push_back(arabian);
		auto temp = make_pair((UINT)Enemy_Entry::Arabian, position);
		objectPoints.push_back(temp);
	}*/

	if (Mouse->Press(2))
	{
		for (size_t i = 0; i < objectList.size(); i++)
		{
			if (position.x < objectList[i]->Position().x + 10.0f &&
				position.x > objectList[i]->Position().x - 10.0f &&
				position.y < objectList[i]->Position().y + 10.0f &&
				position.y > objectList[i]->Position().y - 10.0f)
			{
				objectList[i]->Position(position);
			}
		}
	}

	if (Mouse->Down(1))
	{
		for (size_t i = 0; i < objectList.size(); i++)
		{
			if (position.x < objectList[i]->Position().x + 10.0f &&
				position.x > objectList[i]->Position().x - 10.0f &&
				position.y < objectList[i]->Position().y + 10.0f &&
				position.y > objectList[i]->Position().y - 10.0f)
			{
				auto arabian = objectList.at(i);
				objectList.erase(objectList.begin() + i);
				delete arabian;
				objectPoints.erase(objectPoints.begin() + i);
			}
		}
	}
}

void MapEditor::LoadSpriteDialog()
{
	std::function<void(wstring)> func = CC_CALLBACK_1(MapEditor::OpenComplete, this);
	Path::OpenFileDialog(L"", L"Texture\0*.png", L"./_Textures", func, Hwnd);
}

void MapEditor::OpenComplete(wstring name)
{

}

void MapEditor::CameraMove(D3DXVECTOR2 destination)
{
	auto camera = Director::getInstance()->MainCamera();
	auto position = camera->Position();
	position += destination * Time::deltaTime();
	((Node*)camera)->Position(position);
}


void MapEditor::RenderImGui()
{
	ImGui::LabelText("Mouse", "%0.2f, %0.2f", position.x, position.y);

	static int combo_item = 0;
	bool valueChanged = ImGui::Combo("Object", &combo_item,"None\0Line\0Sprite\0\0");//DropDownMenu

	if (combo_item == 1)
		EditLine();
	else if (combo_item == 2)
		LoadSpriteDialog();

	//---------------------------------------------------------
	//Button
	//---------------------------------------------------------

	//Line
	if (ImGui::Button("Save LIne"))
	{
		FileManager::SetLineData(linePoints);
		FileManager::SetObjectData(objectPoints);
		FileManager::Save(L"Stage1.bin");
	}
	ImGui::SameLine();
	//if (ImGui::Button("Load Line"))
	//{
	//	for (auto line : _scene->lines)
	//		SAFE_DELETE(line);

	//	_scene->lines.clear();
	//	linePoints.clear();

	//	objectList.clear();
	//	objectPoints.clear();

	//	FileManager::Load(L"Stage1.bin");
	//	linePoints = FileManager::GetLineData();
	//	objectPoints = FileManager::GetObjectData();

	//	for (size_t i = 0; i < linePoints.size(); i++)
	//	{
	//		_scene->lines.push_back(new Line(linePoints[i].first,linePoints[i].second));
	//	}

	//	for (size_t i = 0; i < objectPoints.size(); i++)
	//	{
	//		/*switch ((Enemy_Entry)objectPoints[i].first)
	//		{
	//		case Enemy_Entry::Arabian:
	//		{
	//			auto arabian = new Arabian(nullptr,objectPoints[i].second, D3DXVECTOR2(4, 4));
	//			objectList.push_back(arabian);
	//			arabian->PhysicsActive(false);
	//			break;
	//		}
	//		default:
	//			break;
	//		}*/
	//	}
	//}

}

void MapEditor::RenderDebug()
{
	ImGui::Begin("Debug");
	ImGui::LabelText("Line Count", "%d", _scene->lines.size());
	ImGui::LabelText("Line Points", "%d", linePoints.size());

	/*for (auto line : _scene->lines)
	{
		ImGui::LabelText("Line Pos", "(%0.2f, %0.2f)->(%0.2f, %0.2f)", line->LeftPoint().x,line->LeftPoint().y,line->RightPoint().x,line->RightPoint().y);
	}*/

	for (auto line : linePoints)
	{
		ImGui::LabelText("Line Pos", "(%0.2f, %0.2f)->(%0.2f, %0.2f)", line.first.x, line.first.y, line.second.x, line.second.y);
	}

	ImGui::End();
}

#include "stdafx.h"
#include "MapEditor.h"
#include "./Viewer/FreeCamera.h"

#include "./Renders/Line.h"
#include "./Object/Ring.h"
#include "./Object/Fish.h"
#include "./Object/Mole.h"

#include "FileManager.h"

MapEditor::MapEditor(SceneValues * values)
	:Scene(values), mouse(0,0), cameraPos(0,0), position(0,0)
{
	wstring shaderFile = Shaders + L"Effect.fx";
	wstring textureFile = Textures + L"Sonic/sonicStage.png";

	background = new Sprite(textureFile, shaderFile);
	background->Position(0, 0);

	SAFE_DELETE(values->MainCamera);
	values->MainCamera = new FreeCamera();
	((FreeCamera*)values->MainCamera)->Speed(500.0f);
}

MapEditor::~MapEditor()
{
	SAFE_DELETE(background);
	SAFE_DELETE(values->MainCamera);

	for (Line* line : lines)
		SAFE_DELETE(line);

	for (Ring* ring : rings)
		SAFE_DELETE(ring);
}

void MapEditor::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	for (Line* line : lines)
		line->Update(V, P);

	for (Ring* ring : rings)
		ring->Update(V, P);

	for (Fish* fish : fishes)
		fish->Update(V, P);

	for (Mole* mole : moles)
		mole->Update(V, P);


	//마우스 위치 구하기
	mouse = Mouse->Position();
	mouse.x = mouse.x - (float)Width * 0.5f;
	mouse.y = (mouse.y - (float)Height * 0.5f) * -1.0f;
	cameraPos = values->MainCamera->Position();
	position = mouse + cameraPos;

	background->Update(V, P);
}

void MapEditor::Render()
{
	RenderImGui();
	RenderDebug();

	background->Render();

	for (Line* line : lines)
		line->Render();

	for (Ring* ring : rings)
		ring->Render();

	for (Fish* fish : fishes)
		fish->Render();

	for (Mole* mole : moles)
		mole->Render();
}


void MapEditor::EditLine()
{
	bool hovering = ImGui::GetIO().WantCaptureMouse;

	if (Mouse->Down(0) && hovering == false)
	{
		lastPosition = position;
	}

	else if (Mouse->Up(0) && hovering == false)
	{
		if (Key->Press(VK_SHIFT))
		{
			position.y = lastPosition.y;
			lines.push_back(new Line(lastPosition, position));
			linePoints.push_back(pair<D3DXVECTOR2, D3DXVECTOR2>(lastPosition, position));
		}
		else
		{
			lines.push_back(new Line(lastPosition, position));
			linePoints.push_back(pair<D3DXVECTOR2, D3DXVECTOR2>(lastPosition, position));
		}
		
	}

	if (Mouse->Press(2))
	{
		for (size_t i = 0; i < lines.size(); i++)
		{
			if (position.x < lines[i]->LeftPoint().x + 10.0f &&
				position.x > lines[i]->LeftPoint().x - 10.0f &&
				position.y < lines[i]->LeftPoint().y + 10.0f &&
				position.y > lines[i]->LeftPoint().y - 10.0f)
			{
				lines[i]->LeftPoint(position);
				linePoints[i].first = position;
			}

			if (position.x < lines[i]->RightPoint().x + 10.0f &&
				position.x > lines[i]->RightPoint().x - 10.0f &&
				position.y < lines[i]->RightPoint().y + 10.0f &&
				position.y > lines[i]->RightPoint().y - 10.0f)
			{
				lines[i]->RightPoint(position);
				linePoints[i].second = position;
			}
		}
	}

	if (Mouse->Down(1))
	{
		for (size_t i = 0; i < lines.size(); i++)
		{
			if (position.x < lines[i]->RightPoint().x + 10.0f &&
				position.x > lines[i]->RightPoint().x - 10.0f &&
				position.y < lines[i]->RightPoint().y + 10.0f &&
				position.y > lines[i]->RightPoint().y - 10.0f)
			{
				lines.erase(lines.begin() + i);
				linePoints.erase(linePoints.begin() + i);
			}
		}

	}
}

void MapEditor::EdigRing()
{

	bool hovering = ImGui::GetIO().WantCaptureMouse;

	if (Mouse->Down(0) && hovering == false)
	{
		rings.push_back(new Ring(position));
		ringPoints.push_back(position);
	}

	if (Mouse->Press(2))
	{
		for (size_t i = 0; i < rings.size(); i++)
		{
			if (position.x < rings[i]->Position().x + 10.0f &&
				position.x > rings[i]->Position().x - 10.0f &&
				position.y < rings[i]->Position().y + 10.0f &&
				position.y > rings[i]->Position().y - 10.0f)

			{
				rings[i]->Position(position);
				ringPoints[i] = position;
			}
		}
	}
	
	if (Mouse->Down(1))
	{
		for (size_t i = 0; i < rings.size(); i++)
		{
			if (position.x < rings[i]->Position().x + 10.0f &&
				position.x > rings[i]->Position().x - 10.0f &&
				position.y < rings[i]->Position().y + 10.0f &&
				position.y > rings[i]->Position().y - 10.0f)

			{
				rings.erase(rings.begin() + i);
				ringPoints.erase(ringPoints.begin() + i);
			}
		}
	}
	

}

void MapEditor::EditFish()
{	
	bool hovering = ImGui::GetIO().WantCaptureMouse;

	if (Mouse->Down(0) && hovering == false)
	{
		fishes.push_back(new Fish(position));
		fishPoints.push_back(position);
	}

	if (Mouse->Press(2))
	{
		for (size_t i = 0; i < fishes.size(); i++)
		{
			if (position.x < fishes[i]->Position().x + 10.0f &&
				position.x > fishes[i]->Position().x - 10.0f &&
				position.y < fishes[i]->Position().y + 10.0f &&
				position.y > fishes[i]->Position().y - 10.0f)

			{
				fishes[i]->Position(position);
				fishPoints[i] = position;
			}
		}
	}

	if (Mouse->Down(1))
	{
		for (size_t i = 0; i < fishes.size(); i++)
		{
			if (position.x < fishes[i]->Position().x + 10.0f &&
				position.x > fishes[i]->Position().x - 10.0f &&
				position.y < fishes[i]->Position().y + 10.0f &&
				position.y > fishes[i]->Position().y - 10.0f)

			{
				fishes.erase(fishes.begin() + i);
				fishPoints.erase(fishPoints.begin() + i);
			}
		}
	}
}

void MapEditor::EditMole()
{
	bool hovering = ImGui::GetIO().WantCaptureMouse;

	if (Mouse->Down(0) && hovering == false)
	{
		moles.push_back(new Mole(position));
		molePoints.push_back(position);
	}

	if (Mouse->Press(2))
	{
		for (size_t i = 0; i < moles.size(); i++)
		{
			if (position.x < moles[i]->Position().x + 10.0f &&
				position.x > moles[i]->Position().x - 10.0f &&
				position.y < moles[i]->Position().y + 10.0f &&
				position.y > moles[i]->Position().y - 10.0f)

			{
				moles[i]->Position(position);
				molePoints[i] = position;
			}
		}
	}

	if (Mouse->Down(1))
	{
		for (size_t i = 0; i < moles.size(); i++)
		{
			if (position.x < moles[i]->Position().x + 10.0f &&
				position.x > moles[i]->Position().x - 10.0f &&
				position.y < moles[i]->Position().y + 10.0f &&
				position.y > moles[i]->Position().y - 10.0f)

			{
				moles.erase(moles.begin() + i);
				molePoints.erase(molePoints.begin() + i);
			}
		}
	}
}

void MapEditor::RenderImGui()
{
	static int combo_item = 0;
	bool valueChanged = ImGui::Combo("Object", &combo_item, "None\0Line\0Ring\0Fish\0Mole\0\0");

	if (combo_item == 1)
		EditLine();
	else if (combo_item == 2)
		EdigRing();
	else if (combo_item == 3)
		EditFish();
	else if (combo_item == 4)
		EditMole();

	//----------------------------------------------------------------
	// Button
	//----------------------------------------------------------------

	//Line
	if (ImGui::Button("Save Line") == true)
	{
		FileManager::SetMarker2(linePoints);
		FileManager::SaveLine();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Line") == true)
	{
		for (size_t i = 0; i < lines.size(); i++)
			SAFE_DELETE(lines[i]);
		lines.clear();
		linePoints.clear();

		FileManager::LoadLine();
		linePoints = FileManager::GetMarker2();

		for (size_t i = 0; i < linePoints.size(); i++)
			lines.push_back(new Line(linePoints[i].first, linePoints[i].second));
	}

	//Ring
	if (ImGui::Button("Save Ring") == true)
	{
		FileManager::SetMarker(ringPoints);
		FileManager::Save(L"ring.bin");
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Ring") == true)
	{
		for (size_t i = 0; i < rings.size(); i++)
			SAFE_DELETE(rings[i]);
		rings.clear();
		ringPoints.clear();

		FileManager::Load(L"ring.bin");
		ringPoints = FileManager::GetMarker();

		for (size_t i = 0; i < ringPoints.size(); i++)
			rings.push_back(new Ring(ringPoints[i]));
	}

	//Fish
	if (ImGui::Button("Save Fish") == true)
	{
		FileManager::SetMarker(fishPoints);
		FileManager::Save(L"fish.bin");
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Fish") == true)
	{
		for (size_t i = 0; i < fishes.size(); i++)
			SAFE_DELETE(fishes[i]);
		fishes.clear();
		fishPoints.clear();

		FileManager::Load(L"fish.bin");
		fishPoints = FileManager::GetMarker();

		for (size_t i = 0; i < fishPoints.size(); i++)
			fishes.push_back(new Fish(fishPoints[i]));
	}

	//Mole
	if (ImGui::Button("Save Mole") == true)
	{
		FileManager::SetMarker(molePoints);
		FileManager::Save(L"mole.bin");
	}
	ImGui::SameLine();
	if (ImGui::Button("Load Mole") == true)
	{
		for (size_t i = 0; i < moles.size(); i++)
			SAFE_DELETE(moles[i]);
		moles.clear();
		molePoints.clear();

		FileManager::Load(L"mole.bin");
		molePoints = FileManager::GetMarker();

		for (size_t i = 0; i < molePoints.size(); i++)
			moles.push_back(new Mole(molePoints[i]));
	}

}

void MapEditor::RenderDebug()
{
	ImGui::Begin("Debug");
	
	ImGui::LabelText("Line Count", "%d", lines.size());
	ImGui::LabelText("Line Points", "%d", linePoints.size());

	for (Line* line : lines)
	{
		ImGui::LabelText
		(
			"Line Pos", "%0.2f, %0.2f / %0.2f, %0.2f",
			line->LeftPoint().x, line->LeftPoint().y,
			line->RightPoint().x, line->RightPoint().y
		);
	}


	ImGui::End();
}

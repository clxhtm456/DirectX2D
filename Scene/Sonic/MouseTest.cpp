#include "stdafx.h"
#include "MouseTest.h"
#include "Viewer/FreeCamera.h"
#include "./Object/Marker.h"

MouseTest::MouseTest(SceneValues * values)
	:Scene(values)
{
	wstring shaderFile = Shaders + L"Effect.fx";
	wstring textureFile = Textures + L"Sonic/sonicStage.png";

	background = new Sprite(textureFile, shaderFile);
	background->Position(0, 0);

	SAFE_DELETE(values->MainCamera);
	values->MainCamera = new FreeCamera();
	
}

MouseTest::~MouseTest()
{
	for (Marker* marker : markers)
		SAFE_DELETE(marker);

	SAFE_DELETE(background);
	SAFE_DELETE(values->MainCamera);
}

void MouseTest::Update()
{
	D3DXMATRIX V = values->MainCamera->View();
	D3DXMATRIX P = values->Projection;

	for (Marker* marker : markers)
		marker->Update(V, P);

	//마우스 위치 매핑
	D3DXVECTOR2 mouse = Mouse->Position();
	mouse.x = mouse.x - (float)Width * 0.5f;
	mouse.y = (mouse.y - (float)Height * 0.5f) * -1.0f;

	//카메라 위치 보정
	D3DXVECTOR2 camera = values->MainCamera->Position();
	D3DXVECTOR2 position = mouse + camera;

	if (Mouse->Down(0))
		markers.push_back(new Marker(Shaders + L"Effect.fx", position));

	background->Update(V, P);
}

void MouseTest::Render()
{
	//Save TXT
	if (ImGui::Button("Save TXT") == true)
	{
		FILE* fp = NULL;
		fopen_s(&fp, "marker.txt", "w");

		fprintf(fp, "%d\n", markers.size());

		for (size_t i = 0; i < markers.size(); i++)
		{
			fprintf(fp, "%d, %f, %f\n", i, markers[i]->Position().x, markers[i]->Position().y);
		}
		fclose(fp);
	}

	//Load TXT
	ImGui::SameLine();
	if (ImGui::Button("Load TXT"))
	{
		if (Path::ExistFile("marker.txt") == true)
		{
			for (Marker* marker : markers)
				SAFE_DELETE(marker);
			markers.clear();

			FILE* fp;
			fopen_s(&fp, "marker.txt", "r");

			UINT count;
			fscanf_s(fp, "%d", &count);

			for (size_t i = 0; i < count; i++)
			{
				UINT index;
				D3DXVECTOR2 position;

				fscanf_s(fp, "%d, %f, %f", &index, &position.x, &position.y);
				markers.push_back(new Marker(Shaders + L"Effect.fx", position));
			}

			fclose(fp);
		}
	}

	//Save Bin
	if (ImGui::Button("Save Bin") == true)
	{
		BinaryWriter* w = new BinaryWriter();

		w->Open(L"marker.bin");

		w->UInt(markers.size());

		for (size_t i = 0; i < markers.size(); i++)
		{
			w->UInt(i);
			w->Vector2(markers[i]->Position());
		}

		w->Close();
		SAFE_DELETE(w);
	}

	//Load Binary
	ImGui::SameLine();
	if (ImGui::Button("Load Bin") == true)
	{
		if (Path::ExistFile("marker.bin") == true)
		{
			for (Marker* marker : markers)
				SAFE_DELETE(marker);
			markers.clear();

			BinaryReader* r = new BinaryReader();
			r->Open(L"marker.bin");

			UINT count;
			count = r->UInt();

			for (size_t i = 0; i < count; i++)
			{
				UINT index = r->UInt();
				D3DXVECTOR2 position = r->Vector2();

				markers.push_back(new Marker(Shaders + L"Effect.fx", position));
			}

			r->Close();
			SAFE_DELETE(r);
		}
	}

	RenderBackground();

	for (Marker* marker : markers)
		marker->Render();
}

void MouseTest::RenderBackground()
{
	background->Render();
}

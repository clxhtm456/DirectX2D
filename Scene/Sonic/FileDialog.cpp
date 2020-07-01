#include "stdafx.h"
#include "FileDialog.h"
#include "Viewer/FreeCamera.h"
#include "./Object/Marker.h"

FileDialog::FileDialog(SceneValues * values)
	:Scene(values)
{
	wstring shaderFile = Shaders + L"Effect.fx";
	wstring textureFile = Textures + L"Sonic/sonicStage.png";

	background = new Sprite(textureFile, shaderFile);
	background->Position(0, 0);

	SAFE_DELETE(values->MainCamera);
	values->MainCamera = new FreeCamera();

	soundManager = new SoundManager();
	soundManager->AddSound("GUN", "./_Sound/Gun.wav");
	
}

FileDialog::~FileDialog()
{
	for (Marker* marker : markers)
		SAFE_DELETE(marker);

	SAFE_DELETE(background);
	SAFE_DELETE(values->MainCamera);
}

void FileDialog::Update()
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
	{
		soundManager->Play("GUN");
		markers.push_back(new Marker(Shaders + L"Effect.fx", position));
	}
		

	background->Update(V, P);
}

void FileDialog::Render()
{
	//Save Bin
	if (ImGui::Button("Save Bin") == true)
	{
		function<void(wstring)> f = bind(&FileDialog::SaveComplete, this, placeholders::_1);
		Path::SaveFileDialog(L"", L"Binary\0*.bin", L"", f, Hwnd);
	}

	//Load Binary
	ImGui::SameLine();
	if (ImGui::Button("Load Bin") == true)
	{
		function<void(wstring)> f = bind(&FileDialog::OpenComplete, this, placeholders::_1);
		Path::OpenFileDialog(L"", L"Binary\0*.bin", L"", f, Hwnd);
	}

	RenderBackground();

	for (Marker* marker : markers)
		marker->Render();
}

void FileDialog::RenderBackground()
{
	background->Render();
}

void FileDialog::OpenComplete(wstring name)
{
	for (Marker* marker : markers)
		SAFE_DELETE(marker);
	markers.clear();

	BinaryReader* r = new BinaryReader();
	r->Open(name);

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

void FileDialog::SaveComplete(wstring name)
{
	BinaryWriter* w = new BinaryWriter();

	w->Open(name);

	w->UInt(markers.size());

	for (size_t i = 0; i < markers.size(); i++)
	{
		w->UInt(i);
		w->Vector2(markers[i]->Position());
	}

	w->Close();
	SAFE_DELETE(w);

	wstring temp = name + L"\n저장이 완료되었습니다";
	MessageBox(Hwnd, temp.c_str(), L"Save", MB_OK);
}

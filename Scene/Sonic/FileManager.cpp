#include "stdafx.h"
#include "FileManager.h"
#include <algorithm>

vector<D3DXVECTOR2> FileManager::markers = { D3DXVECTOR2(0.0f, 0.0f) };
vector<pair<D3DXVECTOR2, D3DXVECTOR2 >> FileManager::markers2 = { pair<D3DXVECTOR2, D3DXVECTOR2>(D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f)) };

void FileManager::SaveLine()
{
	int reply = MessageBox(Hwnd, L"라인을 저장합니다", L"Save Line", MB_YESNO);

	if (reply = IDYES)
	{
		BinaryWriter* w = new BinaryWriter();
		
		w->Open(L"line.bin");

		sort(markers2.begin(), markers2.end(), FileManager::CompareX2);

		w->UInt(markers2.size());
		w->Byte(&markers2[0], sizeof(pair<D3DXVECTOR2, D3DXVECTOR2>) * markers2.size());
		

		w->Close();
		SAFE_DELETE(w);

		markers2.clear();
		wstring temp = L"line.bin\n저장이 완료되었습니다.";
		MessageBox(Hwnd, temp.c_str(), L"저장완료",  MB_OK);
	}
	else if (reply = IDNO)
	{
		//아니오 누를 경우 코드 작성
	}


}

void FileManager::LoadLine()
{
	BinaryReader* r = new BinaryReader();

	if (Path::ExistFile("line.bin") == true)
		r->Open(L"line.bin");
	else
		assert(false);
	UINT count;
	count = r->UInt();

	vector<pair<D3DXVECTOR2, D3DXVECTOR2>> v;
	v.assign(count, pair<D3DXVECTOR2, D3DXVECTOR2>());

	void* ptr = (void*)&(v[0]);
	//void* ptr = ((void*)v.begin());
	r->Byte(&ptr, sizeof(pair<D3DXVECTOR2, D3DXVECTOR2>) * count);

	r->Close();
	SAFE_DELETE(r);

	SetMarker2(v);
}

void FileManager::SetMarker2(vector<pair<D3DXVECTOR2, D3DXVECTOR2>>& v)
{
	markers2.clear();
	markers2.assign(v.begin(), v.end());
}

vector<pair<D3DXVECTOR2, D3DXVECTOR2>> FileManager::GetMarker2()
{
	return markers2;
}

void FileManager::Save(wstring fileName)
{
	int reply = MessageBox(Hwnd, L"저장할래요?", L"Save Object", MB_YESNO);

	if (reply == IDYES)
	{
		BinaryWriter* w = new BinaryWriter();
		string str = String::ToString(fileName);

		if (strcmp(str.c_str(), "line.bin") != 0)
			w->Open(fileName);
		else
			assert(false);
		sort(markers.begin(), markers.end(), FileManager::CompareX);

		w->UInt(markers.size());
		w->Byte(&markers[0], sizeof(D3DXVECTOR2) * markers.size());

		w->Close();
		SAFE_DELETE(w);

		markers.clear();

		wstring temp = fileName + L"\n저장이 완료되었습니다";
		MessageBox(Hwnd, temp.c_str(), L"저장완료", MB_OK);
	}
}

void FileManager::Load(wstring fileName)
{
	string str = String::ToString(fileName);

	BinaryReader* r = new BinaryReader();

	if (Path::ExistFile(fileName) == true)
		r->Open(fileName);
	else
		assert(false);

	UINT count;
	count = r->UInt();

	vector<D3DXVECTOR2> v;
	v.assign(count, D3DXVECTOR2());

	void* ptr = (void*)&(v[0]);
	r->Byte(&ptr, sizeof(D3DXVECTOR2) * count);

	r->Close();

	SetMarker(v);

}

void FileManager::SetMarker(vector<D3DXVECTOR2>& v)
{
	markers.clear();
	markers.assign(v.begin(), v.end());
}

vector<D3DXVECTOR2> FileManager::GetMarker()
{
	return markers;
}

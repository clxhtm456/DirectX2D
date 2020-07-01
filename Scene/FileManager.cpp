#include "stdafx.h"
#include "FileManager.h"
#include <algorithm>

#define BINARYFILENAME L"line.bin"

vector<wstring> FileManager::textureName = { L"" };
vector<D3DXVECTOR2> FileManager::texturePosition = { D3DXVECTOR2 (0.0f,0.0f)};
vector<D3DXVECTOR2> FileManager::textureScale = { D3DXVECTOR2(0.0f,0.0f) };

vector<pair<D3DXVECTOR2, D3DXVECTOR2>> FileManager::lineData = { pair<D3DXVECTOR2, D3DXVECTOR2> (D3DXVECTOR2(0.0f,0.0f),D3DXVECTOR2(0.0f,0.0f)) };
vector<pair<UINT, D3DXVECTOR2>> FileManager::objectData = { pair<UINT, D3DXVECTOR2>(0,D3DXVECTOR2(0.0f,0.0f)) };



void FileManager::SetLineData(vector<pair<D3DXVECTOR2, D3DXVECTOR2>>& v)
{
	lineData.clear();
	lineData.assign(v.begin(), v.end());
}

void FileManager::SetTextureName(vector<wstring>& v)
{
	textureName.clear();
	textureName.assign(v.begin(), v.end());
}

void FileManager::SetTexturePos(vector<D3DXVECTOR2>& v)
{
	texturePosition.clear();
	texturePosition.assign(v.begin(), v.end());
}

void FileManager::SetTextureScale(vector<D3DXVECTOR2>& v)
{
	textureScale.clear();
	textureScale.assign(v.begin(), v.end());
}

vector<pair<D3DXVECTOR2, D3DXVECTOR2>> FileManager::GetLineData()
{
	return lineData;
}

void FileManager::SetObjectData(vector<pair<UINT, D3DXVECTOR2>>& v)
{
	objectData.clear();
	objectData.assign(v.begin(), v.end());
}

vector<pair<UINT, D3DXVECTOR2>> FileManager::GetObjectData()
{
	return objectData;
}

vector<wstring> FileManager::GetTextureName()
{
	return textureName;
}

vector<D3DXVECTOR2> FileManager::GetTexturePos()
{
	return texturePosition;
}

vector<D3DXVECTOR2> FileManager::GetTextureScale()
{
	return textureScale;
}

void FileManager::Save(wstring fileName)
{
	int reply = MessageBox(Hwnd, L"저장하시겠습니까", L"Save Object", MB_YESNO);

	if (reply == IDYES)
	{
		BinaryWriter* bw = new BinaryWriter();
		string str = String::ToString(fileName);

		//if (strcmp(str.c_str(), fileName.c_str()) != 0)
			bw->Open(fileName);
		/*else
			assert(false);*/

		//라인
		{
			//sort(lineData.begin(), lineData.end(), FileManager::CompareX);//Compare 함수
			int size = lineData.size();
			bw->UInt(size);

				bw->Byte(&lineData[0], sizeof(pair<D3DXVECTOR2, D3DXVECTOR2>)*lineData.size());
		}

		//오브젝트
		/*{
			int size = objectData.size();
			bw->UInt(size);

			bw->Byte(&objectData[0], sizeof(pair<UINT,D3DXVECTOR2>)*objectData.size());
		}*/

		

		bw->Close();

		delete bw;
		lineData.clear();
		textureName.clear();
		texturePosition.clear();
		textureScale.clear();
		objectData.clear();

		wstring temp = fileName + L"\n 저장이 완료되었습니다.";
		MessageBox(Hwnd, temp.c_str(), L"저장완료", MB_OK);
	}
}

void FileManager::Load(wstring fileName)
{
	string str = String::ToString(fileName);

	BinaryReader* br = new BinaryReader();

	if (Path::ExistFile(fileName) == true)
		br->Open(fileName);
	else
		assert(false);

	//라인
	{
		UINT count = br->UInt();

		vector<pair<D3DXVECTOR2, D3DXVECTOR2>> v;
		v.assign(count, pair<D3DXVECTOR2, D3DXVECTOR2>());

			void* ptr = (void*)&v[0];
			br->Byte(&ptr, sizeof(pair<D3DXVECTOR2, D3DXVECTOR2>) * count);

			SetLineData(v);
	}


	//오브젝트
	/*{
		UINT count = br->UInt();
		vector<pair<UINT, D3DXVECTOR2>> objectList;
		objectList.assign(count, pair<UINT, D3DXVECTOR2>());

			void* ptr = (void*) & (objectList[0]);
			br->Byte(&ptr, sizeof(pair<UINT, D3DXVECTOR2>) * count);


			SetObjectData(objectList);
	}*/

	br->Close();

	
	delete br;
}

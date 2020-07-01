#pragma once

class FileManager
{
public:

	static void Save(wstring fileName);
	static void Load(wstring fileName);

	static void SetLineData(vector<pair<D3DXVECTOR2, D3DXVECTOR2>>& v);
	static void SetTextureName(vector<wstring>& v);
	static void SetTexturePos(vector<D3DXVECTOR2>& v);
	static void SetTextureScale(vector<D3DXVECTOR2>& v);
	static vector<pair<D3DXVECTOR2, D3DXVECTOR2>> GetLineData();

	static void SetObjectData(vector<pair<UINT, D3DXVECTOR2>>& v);
	static vector<pair<UINT, D3DXVECTOR2>> GetObjectData();

	static vector<wstring> GetTextureName();
	static vector<D3DXVECTOR2> GetTexturePos();
	static vector<D3DXVECTOR2> GetTextureScale();


public:

	static bool CompareX(const D3DXVECTOR2& a, const D3DXVECTOR2& b)
	{
		return a.x < b.x;
	}

	static bool CompareX2(const pair<D3DXVECTOR2, D3DXVECTOR2>& a, const pair<D3DXVECTOR2, D3DXVECTOR2>& b)
	{
		return a.first.x < b.first.x;
	}

private:
	static vector<wstring> textureName;//�ؽ��� �̸�
	static vector<D3DXVECTOR2> texturePosition;//�ؽ��� ��ġ
	static vector<D3DXVECTOR2> textureScale;//�ؽ��� ��ġ
	
	static vector<pair<D3DXVECTOR2, D3DXVECTOR2>> lineData; // ��������
	static vector<pair<UINT, D3DXVECTOR2>> objectData; //������Ʈ Ÿ��/��ġ
};
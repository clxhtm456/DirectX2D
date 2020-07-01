#pragma once

class FileManager
{
private:
	static vector<D3DXVECTOR2> markers; //라인이 아닌
	static vector<pair<D3DXVECTOR2, D3DXVECTOR2 >> markers2; //라인

public:
	static void SaveLine();
	static void LoadLine();
	static void SetMarker2(vector<pair<D3DXVECTOR2, D3DXVECTOR2>>& v);
	static vector<pair<D3DXVECTOR2, D3DXVECTOR2>> GetMarker2();

	static void Save(wstring fileName);
	static void Load(wstring fileName);
	static void SetMarker(vector<D3DXVECTOR2>& v);
	static vector<D3DXVECTOR2> GetMarker();

public:
	static bool CompareX(const D3DXVECTOR2& a, const D3DXVECTOR2& b)
	{
		return a.x < b.x; 
	}

	static bool CompareY(const D3DXVECTOR2& a, const D3DXVECTOR2& b)
	{
		return a.y > b.y;
	}

	static bool CompareX2(const pair<D3DXVECTOR2, D3DXVECTOR2>& a, pair<D3DXVECTOR2, D3DXVECTOR2>& b)
	{
		return a.first.x < b.first.x;
	}

	
};
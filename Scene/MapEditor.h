#pragma once
#include "Scene/Scene.h"

enum class Enemy_Entry
{
	Arabian = 0
};

class MapEditor:public Scene
{
public:
	static MapEditor* Create(Scene* map);
	bool Init(Scene* map);
	MapEditor();
	~MapEditor();

	void PlayScene(bool val);

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	
private:
	void EditLine();
	void EditArabian();
	void LoadSpriteDialog();

	void OpenComplete(wstring name);

	void CameraMove(D3DXVECTOR2 destination);

private:
	void RenderImGui();
	void RenderDebug();

private:
	vector<pair<D3DXVECTOR2, D3DXVECTOR2>> linePoints;

	vector<class Sprite*> spriteList;
	vector<pair<wstring, D3DXVECTOR2>> SpritePoints;

	vector<class PhysicsObject*> objectList;
	vector<pair<UINT, D3DXVECTOR2>> objectPoints;

	D3DXVECTOR2 cursor;
	D3DXVECTOR2 cameraPos;
	D3DXVECTOR2 position;

	D3DXVECTOR2 lastPosition;
	Scene* _scene;

	bool bDrawLine;
	int selectItem;
	UINT selectLinePos;

	
};
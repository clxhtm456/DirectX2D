#pragma once
#include "../Scene.h"

class MapEditor : public Scene
{
private:
	vector<class Line*> lines;
	vector<pair<D3DXVECTOR2, D3DXVECTOR2>> linePoints;

	vector<class Ring*> rings;
	vector<D3DXVECTOR2> ringPoints;

	vector<class Fish*> fishes;
	vector<D3DXVECTOR2> fishPoints;

	vector<class Mole*> moles;
	vector<D3DXVECTOR2> molePoints;

	Sprite* background;

	D3DXVECTOR2 mouse;
	D3DXVECTOR2 cameraPos;
	D3DXVECTOR2 position;

	D3DXVECTOR2 lastPosition;

private:
	void EditLine();
	void EdigRing();
	void EditFish();
	void EditMole();

private:
	void RenderImGui();
	void RenderDebug();

public:
	MapEditor(SceneValues* values);
	~MapEditor();

	void Update() override;
	void Render() override;
};

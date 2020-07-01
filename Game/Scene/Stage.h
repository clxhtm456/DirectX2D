#pragma once

#include "Scene/Scene.h"

class Stage : public Scene
{
public:
	static Stage* Create(wstring mapResource);
	bool Init(wstring mapResource);

	void DrawBackGround();
	void DrawUI();

	Stage();
	~Stage();

	virtual void Update();
	virtual void Render();

	void TestFunction();

	void ObjectCreate(Camera* cam,UINT type,D3DXVECTOR2 vec);
private:
	Sprite * backGround;

	class Player* player;
	class MetalUI* ammoUI;
	
	vector<class Sprite*> textures;
	vector<class Fish*> fishes;
	vector<class Mole*> moles;

	vector<D3DXVECTOR2> ringPoints;
	vector<D3DXVECTOR2> fishPoints;
	vector<D3DXVECTOR2> molePoints;

	Camera* camera;

	

	bool bLineVisible;
	float slope;
	//WaveFunction
	int phase;
	void Wave1Start();
	void Wave1Clear();

	void Wave2Start();
	void Wave2Clear();
};
#pragma once
#include "../Scene.h"

class PlaySonic : public Scene
{
private:
	Sprite * background;
	class Sonic* player;

	vector<class Line*> lines;
	vector<class Ring*> rings;
	vector<class Fish*> fishes;
	vector<class Mole*> moles;

	vector<pair<D3DXVECTOR2, D3DXVECTOR2>> linePoints;
	vector<D3DXVECTOR2> ringPoints;
	vector<D3DXVECTOR2> fishPoints;
	vector<D3DXVECTOR2> molePoints;

	bool lineVisible;
	float slope;

public:
	PlaySonic(SceneValues* values);
	~PlaySonic();

	void Update() override;
	void Render() override;
};

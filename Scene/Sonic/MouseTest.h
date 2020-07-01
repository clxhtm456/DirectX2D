#pragma once
#include "../Scene.h"

class MouseTest : public Scene
{
private:
	Sprite* background;

	vector<class Marker*> markers;

private :
	void RenderBackground();


public:
	MouseTest(SceneValues* values);
	~MouseTest();

	void Update() override;
	void Render() override;

};
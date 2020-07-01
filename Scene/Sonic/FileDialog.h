#pragma once
#include "../Scene.h"

class FileDialog : public Scene
{
private:
	Sprite* background;

	vector<class Marker*> markers;

	SoundManager* soundManager;

private :
	void RenderBackground();
	void OpenComplete(wstring name);
	void SaveComplete(wstring name);

public:
	FileDialog(SceneValues* values);
	~FileDialog();

	void Update() override;
	void Render() override;

};
#pragma once

#include "Viewer/Camera.h"
#include "Scene/Scene.h"

class Director
{
public:
	static Director* getInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new Director();
		}

		return _instance;
	}
	static void Destroy()
	{
		delete _instance;
	}
public:
	void Update();
	void Render();
	Camera* MainCamera() { return Camera::MainCamera(); }
	Scene* GetRunningScene() {
		return _runningScene;
	}
	void AddCamera(Camera* cam);
	void DeleteCamera(Camera* cam);

	void AddReleaseList(Node * b);
	void AddRemoveList(Node* b);

	void AddSceneList(Scene* scene);
	void PlayScene(int index);
	void PlayScene(Scene* scene);

	void ReleasePoolClear();
	void RemovePoolClear();

	void DebugOption(bool val) { debugOption = val; }
	bool DebugOption() { return debugOption; }

	float GetWindowWidth();
	float GetWindowHeight();

	Shader* DefaultShader();
	
private:

	Director();
	~Director();

	vector<Node*> releaseList;
	vector<Node*> removeList;
	vector<Camera*> cameraList;
	vector<Scene*> sceneList;

	Shader* defaultShader = nullptr;

	Scene* _runningScene = nullptr;
	static Director * _instance;
	bool debugOption = false;
};
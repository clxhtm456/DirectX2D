#include "stdafx.h"
#include "Director.h"

#include "Viewer/Camera.h"
#include "Object/Node.h"

Director *  Director::_instance;

void Director::Update()
{
	ReleasePoolClear();

	if (_runningScene != nullptr)
	{
		_runningScene->RenewPhysicsList();
		_runningScene->PostUpdate();
		_runningScene->Update();
		_runningScene->LateUpdate();
	}

	RemovePoolClear();
		
}

void Director::Render()
{
	ImGui::Checkbox("DebugOption", &debugOption);
	if (_runningScene != nullptr)
		_runningScene->Render();
}

void Director::AddCamera(Camera* cam)
{
	cameraList.push_back(cam);
	cam->SetDeleteCallback(CC_CALLBACK_1(Director::DeleteCamera,this));
}

void Director::DeleteCamera(Camera * cam)
{
	for (size_t i = 0; i < cameraList.size(); i++)
	{
		if (cameraList[i] == cam)
		{
			cameraList.erase(cameraList.begin() + i);
		}
	}
}

void Director::AddReleaseList(Node * b)
{
	releaseList.push_back(b);
}

void Director::AddRemoveList(Node* b)
{
	removeList.push_back(b);
}

void Director::AddSceneList(Scene* scene)
{
	sceneList.push_back(scene);
	scene->retain();
	/*if (sceneList.size() == 1)
		PlayScene(0);*/
}

void Director::PlayScene(int index)
{
	if (_runningScene == sceneList[index])
		return;

	if (_runningScene != nullptr)
		_runningScene->PlayScene(false);

	_runningScene = sceneList[index];
	_runningScene->PlayScene(true);
}

void Director::PlayScene(Scene* scene)
{
	if (_runningScene == scene)
		return;

	if (_runningScene != nullptr)
		_runningScene->PlayScene(false);

	_runningScene = scene;
	_runningScene->PlayScene(true);
}

void Director::ReleasePoolClear()
{
	if (releaseList.size() == 0)
		return;

	std::vector<Node*> releasings;
	releasings.swap(releaseList);
	for (const auto& obj : releasings)
	{
		obj->release();
	}
}

void Director::RemovePoolClear()
{
	if (removeList.size() == 0)
		return;

	std::vector<Node*> releasings;
	releasings.swap(removeList);
	for (const auto& obj : releasings)
	{
		obj->removeFromParent();
	}
}

float Director::GetWindowWidth()
{
	return (Width*1/_runningScene->getDefaultCamera()->GetMgnfc());
}

float Director::GetWindowHeight()
{
	return (Height * 1 / _runningScene->getDefaultCamera()->GetMgnfc());
}

Shader* Director::DefaultShader()
{
	if (defaultShader == nullptr)
		defaultShader = new Shader(Shaders + L"Effect.fx");
	return defaultShader;
}

Director::Director()
{
}

Director::~Director()
{
	SAFE_DELETE(defaultShader);
	RemovePoolClear();
	ReleasePoolClear();
	for (auto scene : sceneList)
		scene->release();
}

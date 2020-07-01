#ifndef __SCENE_H__
#define __SCENE_H__

#include "stdafx.h"
#include "Viewer/Camera.h"

class Scene : public Node
{
public:
	Scene();
	virtual ~Scene();
	virtual bool Init();

	virtual void CreateBackGround();
	virtual void CreateGameObject();

	virtual void Update();
	void LateUpdate();
	virtual void Render();

	void PhysicsUpdate();

	void SceneActive(bool val);

	Camera* getDefaultCamera() const { return _defaultCamera; }

	void AddPhysicsObject(class PhysicsObject* object);
	void AddPhysicsLine(class Line* line);
	void DelPhysicsObject(class PhysicsObject* object);
	void DelPhysicsLine(class Line* line);

	void RenewPhysicsList();

	virtual void PlayScene(bool val);
public:
	static Scene* MainScene;
protected:
	bool _sceneActive;
	vector<Camera*> _cameraList;
	Camera* _defaultCamera;

public:
	//physics
	vector<class Line*> lines;
	vector<class PhysicsObject*> physicsObjects;
};

#endif
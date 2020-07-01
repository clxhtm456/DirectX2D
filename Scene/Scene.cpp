#include "stdafx.h"
#include "Scene.h"
#include "Object/PhysicsObject.h"
#include "Renders/Line.h"

Scene::Scene()
{
	_defaultCamera = Camera::Create();
	AddChild(_defaultCamera);
}

Scene::~Scene()
{
}

bool Scene::Init()
{
	if(!Node::Init())
		return false;

	_running = false;

	return true;
}

void Scene::CreateBackGround()
{
}

void Scene::CreateGameObject()
{
}

void Scene::Update()
{
	Camera::MainCamera()->Update();
	PhysicsUpdate();

	Node::Update();
}

void Scene::LateUpdate()
{
	Node::LateUpdate();
}

void Scene::Render()
{
	Node::Render();

	for (auto line : lines)
	{
		line->Render();
	}
}

void Scene::PhysicsUpdate()
{
	for (auto iter = physicsObjects.begin(); iter < physicsObjects.end(); iter++)
	{
		auto object = (*iter);
		//Line Collision--------------------------
		bool lineCollision = false;
		bool leftCollision = false;
		bool rightCollision = false;
		for (auto line : lines)
		{
			auto leftPoint = line->LeftPoint();
			auto rightPoint = line->RightPoint();
			auto slope = (leftPoint.y - rightPoint.y) / (leftPoint.x - rightPoint.x);

			auto WidthPos = (object->ColliderLeftPoint() + object->ColliderRightPoint()) * 0.5f;
			auto objectPos = D3DXVECTOR2(WidthPos, object->ColliderBottomPoint()-1.0f);
			
			
			if (fabs(slope) > 3)
			{
				//-----Wall-------------
				auto objectLeftLine1 = D3DXVECTOR2(object->ColliderLeftPoint(), object->ColliderTopPoint());
				auto objectLeftLine2 = D3DXVECTOR2(object->ColliderLeftPoint(), object->ColliderBottomPoint()+10.0f);

				auto objectRightLine1 = D3DXVECTOR2(object->ColliderRightPoint(), object->ColliderTopPoint());
				auto objectRightLine2 = D3DXVECTOR2(object->ColliderRightPoint(), object->ColliderBottomPoint()+10.0f);

				auto objectBodyPos = D3DXVECTOR2(objectPos.x, object->ColliderBottomPoint());

				bool leftCol = Math::SementInterSect(objectBodyPos, objectLeftLine2, leftPoint, rightPoint);
				bool rightCol = Math::SementInterSect(objectBodyPos, objectRightLine2, leftPoint, rightPoint);

				if (leftCol)
				{
					leftCollision = true;
				}
				if (rightCol)
				{
					rightCollision = true;
				}
				//----------------------
			}
			else
			{
				//Floor----------------

				auto objectBodyPos = D3DXVECTOR2(objectPos.x, objectPos.y + 20);

				auto collision = Math::SementInterSect(objectBodyPos, objectPos, leftPoint, rightPoint);


				if (collision)
				{
					if (object->isLanding())
					{
						line->DrawCollision(true);
						lineCollision = true;
						object->LineLeftPoint(leftPoint);
						object->LineRightPoint(rightPoint);
					}
				}
				else
				{
					line->DrawCollision(false);
					slope = -99.9f;
				}
				//---------------------
			}
			
		}
		object->LineBottomCollision(lineCollision);
		object->LeftCollision(leftCollision);
		object->RightCollision(rightCollision);
		////------------------------------------

		//Object Collision----------------------
		for (auto iter2 = iter+1; iter2 < physicsObjects.end(); iter2++)
		{
			auto object2 = (*iter2);

			if ((object->Flag() & object2->Flag()))
			{
				object->CheckCollision(object2);
				object2->CheckCollision(object);
			}
		}
		////------------------------------------

	}
}


void Scene::SceneActive(bool val)
{
	_sceneActive = val;

	for (auto child : getChildren())
	{
		if (_sceneActive)
			child->OnEnter();
		else
			child->OnExit();
	}
}

void Scene::AddPhysicsObject(PhysicsObject * object)
{
	physicsObjects.push_back(object);
}

void Scene::AddPhysicsLine(Line * line)
{
}

void Scene::DelPhysicsObject(PhysicsObject * object)
{
	for (size_t i = 0; i < physicsObjects.size(); i++)
	{
		if (physicsObjects.at(i) == object)
		{
			physicsObjects.erase(physicsObjects.begin() + i);
		}
	}
}

void Scene::DelPhysicsLine(Line * line)
{
}

void Scene::RenewPhysicsList()
{
	if (PhysicsObject::physicsObjectList.size() == 0)
		return;

	std::vector<PhysicsObject*> addList;
	addList.swap(PhysicsObject::physicsObjectList);
	for (const auto& obj : addList)
	{
		AddPhysicsObject(obj);
	}
}

void Scene::PlayScene(bool val)
{
	_running = val;
}

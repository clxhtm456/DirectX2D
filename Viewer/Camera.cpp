#include "stdafx.h"
#include "Camera.h"

Camera* Camera::_mainCamera;

Camera::Camera():
	depth(0.0f),
	mgnfc(1.0f)
{
	D3DXMatrixIdentity(&_view);//항등행렬화
	D3DXMatrixIdentity(&_projection);

	_mainCamera = this;
}

Camera::~Camera()
{
	if (deleteCallback)
		deleteCallback(this);

	moveCallbackList.clear();
}

Camera * Camera::Create()
{
	Camera* pRet = new Camera();
	pRet->Init();
	pRet->autorelease();

	return pRet;
}

Camera* Camera::MainCamera()
{
	auto scene = Director::getInstance()->GetRunningScene();
	if (scene)
	{
		return scene->getDefaultCamera();
	}
	return nullptr;
}

bool Camera::Init()
{
	return true;
}

void Camera::SetMgnfc(float val)
{
	mgnfc = val;
}

void Camera::SetDeleteCallback(const std::function<void(Camera*)>& callback)
{
	deleteCallback = callback;
}

void Camera::AddMoveCallback(const function<void(float,float)>& func)
{
	moveCallbackList.push_back(func);
}

void Camera::Update()
{
	//View
	D3DXMatrixTranslation(&_view, -_position.x, -_position.y, depth);//2d는 z불필요 , 카메라의 이동과 반대방향으로 오브젝트 view 이동

	D3DXMatrixOrthoOffCenterLH(
		&_projection,
		(float)Width * (1/mgnfc) *-0.5f,
		(float)Width * (1/mgnfc)*0.5f,
		(float)Height *(1/mgnfc)* -0.5f,
		(float)Height *(1/mgnfc)* 0.5f,
		-10,
		10);

	if (_position != oldPosition)
	{
		for (auto callback : moveCallbackList)
		{
			if (callback != NULL)
			{
				float deltaX = oldPosition.x - _position.x;
				float deltaY = oldPosition.y - _position.y;

				callback(deltaX,deltaY);
			}
		}
		oldPosition = _position;
	}
}

void Camera::Render()
{
	Node::Render();
	ImGui::LabelText("CameraPos", "X : %f Y : %f", _position.x, _position.y);
}

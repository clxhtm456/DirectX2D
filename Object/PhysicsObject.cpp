
#include "stdafx.h"
#include "PhysicsObject.h"

#define GRAVITY	-160.0f

vector<PhysicsObject*> PhysicsObject::physicsObjectList;
//vector<PhysicsObject*> PhysicsObject::destroyNode;

bool PhysicsObject::Init()
{
	if (!Node::Init())
		return false;

	collider = new PhysicsBox();
	rect = new Rect(Shaders + L"Rect.fx", Position(), D3DXVECTOR2(5, 5));

	rect->Color(1, 0, 0);

	Reset();
	_flag = 0;
	gravity = GRAVITY;
	isKinematic = false;
	
	collisionList.clear();
	physicsObjectList.push_back(this);
	//Director::getInstance()->GetRunningScene()->AddPhysicsObject(this);

	BoundVertex vertices[5];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0);
	vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0);
	vertices[2].Position = D3DXVECTOR3(0.5f, 0.5f, 0);
	vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0);
	vertices[4].Position = D3DXVECTOR3(-0.5f, -0.5f, 0);

	boundShader = new Shader(Shaders + L"Bounding.fx");

	//Create BoundVertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(BoundVertex) * 5;//개수 5개
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &boundVertexBuffer);
		assert(SUCCEEDED(hr));
	}

	return true;
}

void PhysicsObject::Reset()
{
	degree = D3DXVECTOR3(0, 0, 0);
	
	velocity = 0.0f;
	bGround = false;
	bLineBottonCollision = false;
	bActive = true;
	

	leftCollision = false;
	rightCollision = false;
	bottomCollision = false;
	topCollision = false;
	objleftCollision = false;
	objrightCollision = false;
	objtopCollision = false;
	objbottomCollision = false;
	

	rect->Position(_normalizedPosition);
}

PhysicsObject::~PhysicsObject()
{
	if (PhysicsObject::physicsObjectList.size() != 0)
	{
		for (size_t i = 0; i < physicsObjectList.size(); i++)
		{
			if (physicsObjectList.at(i) == this)
			{
				physicsObjectList.erase(physicsObjectList.begin() + i);
			}
		}
	}

	if (Director::getInstance()->GetRunningScene() != nullptr)
		Director::getInstance()->GetRunningScene()->DelPhysicsObject(this);

	for (auto col : collisionList)
	{
		//ExitCollisionAll(col);
		col->ExitCollision(this);
	}

	collisionList.clear();

	delete collider;
	delete boundShader;
	delete rect;
}

void PhysicsObject::LineBottomCollision(bool val)
{
	bLineBottonCollision = val;
}

bool PhysicsObject::Aabb(D3DXVECTOR2 targetPosition)
{
	auto xScale = _scale.x * collider->Width() ;
	auto yScale = _scale.y * collider->Height();

	float left = _position.x - xScale * AnchorPosition().x;
	float right = _position.x + xScale * (1-AnchorPosition().x);
	float bottom = _position.y - yScale * AnchorPosition().y;
	float top = _position.y + yScale * (1 - AnchorPosition().y);

	bool bCheck = true;
	bCheck &= targetPosition.x >= left;
	bCheck &= targetPosition.x <= right;
	bCheck &= targetPosition.y >= bottom;
	bCheck &= targetPosition.y <= top;

	return bCheck;
}

bool PhysicsObject::Aabb(PhysicsObject * b, OUT ColDirection& direction)
{
	auto xScale = _normalizedScale.x* collider->Width();
	auto yScale = _normalizedScale.y* collider->Height();

	float leftA = _normalizedPosition.x - xScale * AnchorPosition().x;
	float rightA = _normalizedPosition.x + xScale * (1 - AnchorPosition().x);
	float bottomA = _normalizedPosition.y - yScale * AnchorPosition().y;
	float topA = _normalizedPosition.y + yScale * (1 - AnchorPosition().y);

	xScale = b->_normalizedScale.x * b->collider->Width();
	yScale = b->_normalizedScale.y* b->collider->Height();

	float leftB = b->NormalizedPosition().x - xScale * b->AnchorPosition().x;
	float rightB = b->NormalizedPosition().x + xScale * (1 - b->AnchorPosition().x);
	float bottomB = b->NormalizedPosition().y - yScale * b->AnchorPosition().y;
	float topB = b->NormalizedPosition().y + yScale * (1 - b->AnchorPosition().y);

	bool bCheck = true;
	bCheck &= leftA <= rightB;
	bCheck &= rightA >= leftB;
	bCheck &= topA >= bottomB;
	bCheck &= bottomA <= topB;
	if (bCheck)
	{
		auto left = abs(leftA - rightB);
		auto right = abs(rightA - leftB);
		auto up = abs(topA - bottomB);
		auto bottom = abs(bottomA - topB);

		auto min = left;
		direction = ColDirection::LEFT;
		if (right < min)
		{
			direction = ColDirection::RIGHT;
			min = right;
		}
		if (up < min)
		{
			direction = ColDirection::UP;
			min = up;
		}
		if (bottom < min)
		{
			direction = ColDirection::DOWN;
			min = bottom;
		}
	}

	return bCheck;
}

bool PhysicsObject::CheckCollision(PhysicsObject* b)
{
	if (_running == false || b->GetRunning() == false)
		return false;

	ColDirection direction;
	bool firstCheck = true;
	if (Aabb(b, direction))
	{
		for (auto col : collisionList)
		{
			if (col == b)
			{
				firstCheck = false;
			}
		}
		if (firstCheck == true)
		{
			EnterCollision(b, direction);
		}
		else
		{
			OnPhysicsCollision(b, direction);
		}
		return true;
	}
	else
	{
		auto size = collisionList.size();
		ExitCollision(b);
		return false;
	}
}

void PhysicsObject::EnterCollision(PhysicsObject * b, ColDirection direction)
{
	EnterPhysicsCollision(b, direction);
	collisionList.push_back(b);
}

void PhysicsObject::ExitCollision(PhysicsObject * b)
{
	auto size = collisionList.size();
	if (size > 0)
	{
		for (size_t i = 0; i < size; i++)
		{
			if (collisionList[i] == b)
			{
				ExitPhysicsCollision(b);
				collisionList.erase(collisionList.begin() + i);
				break;
			}
		}
	}
}

void PhysicsObject::ExitCollisionAll()
{
	for (size_t i = 0; i < collisionList.size(); i++)
	{
		ExitPhysicsCollision(collisionList[i]);
		collisionList.erase(collisionList.begin() + i);
	}
}

void PhysicsObject::Destroy(PhysicsObject* node)
{
}

float PhysicsObject::ColliderLeftPoint()
{
	auto tempAnchor = _anchorPoint;
	{
		tempAnchor.x = abs(_anchorPoint.x - (_normalizedRotation.y / Math::PI));
	}

	return (_normalizedPosition.x + collider->Position().x - _normalizedScale.x * collider->Width() * (tempAnchor.x));
}

float PhysicsObject::ColliderRightPoint()
{
	auto tempAnchor = _anchorPoint;
	{
		tempAnchor.x = abs(_anchorPoint.x - (_normalizedRotation.y / Math::PI));
	}

	return (_normalizedPosition.x + collider->Position().x + _normalizedScale.x * collider->Width() * (1.0f-(tempAnchor.x)));
}

float PhysicsObject::ColliderTopPoint()
{
	auto tempAnchor = _anchorPoint;
	{
		tempAnchor.y = abs(_anchorPoint.y - (_normalizedRotation.x / Math::PI));
	}

	return (_normalizedPosition.y + collider->Position().y + _normalizedScale.y * collider->Height() * (1.0f - (tempAnchor.y)));
}

float PhysicsObject::ColliderBottomPoint()
{
	auto tempAnchor = _anchorPoint;
	{
		tempAnchor.y = abs(_anchorPoint.y - (_normalizedRotation.x / Math::PI));
	}

	return (_normalizedPosition.y + collider->Position().y - _normalizedScale.y * collider->Height() * (tempAnchor.y));
}

void PhysicsObject::PostUpdate()
{
	Node::PostUpdate();
	currentPosition = Position();
}

void PhysicsObject::Update()
{
	if (!bActive)
		return; 

	rect->Update();
	rect->Position(_normalizedPosition);

	if (!isKinematic)
	{
		auto position = Position();

		position += moveAccel * Time::deltaTime();

		//라인충돌
		if (bLineBottonCollision)
		{
			auto WidthPos = (ColliderLeftPoint() + ColliderRightPoint()) * 0.5f;
			auto objectPos = D3DXVECTOR2(WidthPos, ColliderBottomPoint());
			auto objectBodyPos = D3DXVECTOR2(objectPos.x, objectPos.y + 20);

			float slope = (linePoint.second.y - linePoint.first.y) / (linePoint.second.x - linePoint.first.x);
			auto goalY = (slope * (objectPos.x - linePoint.first.x) + linePoint.first.y + (ColliderTopPoint() - ColliderBottomPoint()) * AnchorPosition().y);
			position.y = goalY;
		}
		else
		{
			bGround = false;
			//degree.z = 0.0f;
		}

		//y축 가속도 업데이트
		velocity += gravity * Time::deltaTime() * _scale.y;

		//점프
		if (bGround == false)
		{
			position.y += velocity * Time::deltaTime() * _scale.y;
		}

		if (bLineBottonCollision || ObjBottomCollision())
		{
			velocity = 0.0f;
			bGround = true;
		}
		Position(position);
	}

	Node::Update();
}

void PhysicsObject::LateUpdate()
{
	Node::LateUpdate();
	auto position = Position();
	if (leftCollision || objleftCollision)
	{
		if (position.x < currentPosition.x)
		{
			position.x = currentPosition.x;
		}
	}

	if (rightCollision || objrightCollision)
	{
		if (position.x > currentPosition.x)
		{
			position.x = currentPosition.x;
		}
	}

	if (topCollision || objtopCollision)
	{
		if (position.y > currentPosition.y)
		{
			position.y = currentPosition.y;
		}
	}

	if (bottomCollision || objbottomCollision)
	{
		if (position.y < currentPosition.y)
		{
			position.y = currentPosition.y;
		}
		velocity = 0.0f;
		bGround = true;
	}

	Position(position);
}

void PhysicsObject::Render()
{
	Node::Render();
	

	if (Director::getInstance()->DebugOption() == true)
	{
		rect->Render();
		DrawBoundingBox();
	}
	ImGui::LabelText("position", "%f , %f", (Position().x),(Position().y));
}

void PhysicsObject::DrawBoundingBox()
{
	D3DXMATRIX V = *Director::getInstance()->MainCamera()->GetView();
	D3DXMATRIX P = *Director::getInstance()->MainCamera()->GetProjection();

	boundShader->AsMatrix("View")->SetMatrix(V);
	boundShader->AsMatrix("Projection")->SetMatrix(P);

	D3DXMATRIX S, T, R;

	D3DXMatrixScaling(&S, _normalizedScale.x * collider->Width(), _normalizedScale.y * collider->Height(), 1.0f);//z값은 1은 넣어주기
	D3DXMatrixRotationYawPitchRoll(&R, _normalizedRotation.y, _normalizedRotation.x, _normalizedRotation.z);
	auto tempAnchor = _anchorPoint;
	{
		tempAnchor.x = abs(_anchorPoint.x - (_normalizedRotation.y / Math::PI));
		tempAnchor.y = abs(_anchorPoint.y - (_normalizedRotation.x / Math::PI));
	}
	//auto tempDepth = pow(2.72, -depth);
	D3DXMatrixTranslation(&T, (_normalizedPosition.x+collider->Position().x + _normalizedScale.x * collider->Width() * (0.5f - tempAnchor.x)),
		(_normalizedPosition.y + collider->Position().y + _normalizedScale.y * collider->Height() * (0.5f - tempAnchor.y)),
		0.0f);

	world = S * R * T;//scale * translation 순서유의

	boundShader->AsMatrix("World")->SetMatrix(world);

	UINT stride = sizeof(BoundVertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &boundVertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINESTRIP);

	if(leftCollision || rightCollision)
		boundShader->Draw(0, 1, 5);
	else
		boundShader->Draw(0, 0, 5);
	//BoundingBox 그리기

}

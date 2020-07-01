#include "stdafx.h"
#include "Node.h"

Node::Node() : _referenceCount(1),
_position(D3DXVECTOR2(0, 0)),
_normalizedPosition(D3DXVECTOR2(0, 0)),
_scale(D3DXVECTOR2(1, 1)),
_normalizedScale(D3DXVECTOR2(1, 1)),
_rotation(D3DXVECTOR3(0, 0, 0)),
_normalizedRotation(D3DXVECTOR3(0, 0, 0)),
_anchorPoint(D3DXVECTOR2(0.5, 0.5)),
_running(true),
_reorderChildDirty(false),
_visible(true),
_tag (0)
{
	_director = Director::getInstance();
	_parent = nullptr;
}

Node::~Node()
{
	for (auto object : _childList)
		object->release();
}

Node* Node::Create()
{
	Node* pRet = new Node();
	if (pRet && pRet->Init())
	{
		pRet->autorelease();

	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

bool Node::Init()
{
	/*_position = (D3DXVECTOR2(0, 0));
	_normalizedPosition = _position;
	_scale = (D3DXVECTOR2(1, 1));
	_normalizedScale = _scale;
	_rotation = (D3DXVECTOR3(0, 0, 0));
	_normalizedRotation = _rotation;
	_anchorPoint = D3DXVECTOR2(0.5, 0.5);
	_referenceCount = 1;
	_running = true;
	_reorderChildDirty = false;
	_visible = true;
	_tag = 0;

	_director = Director::getInstance();
	_parent = nullptr;*/

	return true;
}

void Node::PostUpdate()
{
	auto list = _childList;
	for (auto object : list)
		object->PostUpdate();
}

void Node::Update()
{
	for (int i = 0; i < eventList.size(); i++)
	{
		if (eventList[i].second <= 0)
		{
			if (eventList[i].first != nullptr)
				eventList[i].first();
			eventList.erase(eventList.begin()+i);
		}
		else
		{
			eventList[i].second -= Time::deltaTime();
		}
	}
	/*for (auto iter = eventList.begin(); iter != eventList.end(); iter++)
	{
		if ((*iter).second <= 0)
		{
			if ((*iter).first != nullptr)
				(*iter).first();
			iter = eventList.erase(iter);
		}
		else
		{
			(*iter).second -= Time::deltaTime();
		}
	}*/

	auto list = _childList;
	for (auto object : list)
	{
		if(object->GetRunning())
			object->Update();
	}
}

void Node::LateUpdate()
{
	auto list = _childList;
	for (auto object : list)
		object->LateUpdate();
}

void Node::Render()
{
	if (_visible == false)
		return;

	Draw();

	sortAllChildren();

	auto list = _childList;
	for (auto object : list)
		object->Render();
}

void Node::Draw()
{
}

void Node::AddChild(Node* child, int _localZOrder)
{
	for (auto _child : _childList)
	{
		if (_child == child)
			return;
	}
	_childList.push_back(child);
	_reorderChildDirty = true;
	child->_setLocalZOrder(_localZOrder);
	auto pos = child->Position();
	auto rot = child->Rotation();
	auto scale = child->Scale();
	child->_parent = this;
	child->Position(pos);
	child->Rotation(rot);
	child->Scale(scale);
	child->retain();
}

void Node::AddChild(Node * child)
{
	AddChild(child, child->getLocalZOrder());
}

void Node::removeFromParent()
{
	if (_parent != nullptr)
	{
		_parent->removeChild(this);
	}
}

void Node::removeChild(Node * child, bool cleanup)
{
	if (_childList.empty())
	{
		return;
	}

	int index;
	auto iter = std::find(_childList.begin(), _childList.end(), child);
	if (iter != _childList.end())
	{
		this->detachChild(child,(iter- _childList.begin()), cleanup);
	}
	_reorderChildDirty = true;
}

void Node::Reset()
{
}

void Node::OnEnter()
{
	for (const auto &child : _childList)
		child->OnEnter();

	_running = true;
}

void Node::OnExit()
{
	_running = false;

	for (const auto &child : _childList)
		child->OnExit();
}

void Node::SetParent(Node * parent)
{
	_parent = parent;
}

void Node::detachChild(Node * child, SSIZE_T index, bool doCleanup)
{
	if (_running)
	{
		child->OnExit();
	}

	child->SetParent(nullptr);

	auto it = std::next(_childList.begin(), index);
	(*it)->release();
	_childList.erase(it);
}

void Node::sortAllChildren()
{
	if (_reorderChildDirty)
	{
		std::sort(std::begin(_childList), std::end(_childList), [](Node* n1, Node* n2) {
			return (n1->_localZOrder == n2->_localZOrder && n1->_orderOfArrival < n2->_orderOfArrival) || n1->_localZOrder < n2->_localZOrder;
			});
		_reorderChildDirty = false;
	}
}

void Node::RemoveSelf()
{
	Director::getInstance()->AddRemoveList(this);
}

void Node::SetInvisible()
{
	_visible = false;
}

void Node::SetVisible()
{
	_visible = true;
}

bool Node::IsVisible()
{
	return _visible;
}

Node * Node::Parent()
{
	return _parent;
}

void Node::Position(float x, float y)
{
	//프로젝트 -> 옵션 ->C++->언어 - >준수모드 아니오
	Position(D3DXVECTOR2(x, y));
}

void Node::Position(D3DXVECTOR2 vec)
{
	D3DXVECTOR2 parentPos = D3DXVECTOR2(0, 0);
	D3DXVECTOR3 parentRot = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR2 parentScale = D3DXVECTOR2(1, 1);
	if (_parent != nullptr)
	{
		parentPos = _parent->_normalizedPosition;
		parentRot = _parent->_normalizedRotation;
		parentScale = _parent->_normalizedScale;
	}
	
	auto direction = D3DXVECTOR2((parentRot.y / Math::PI) > 0 ? -1:1, (parentRot.x / Math::PI) > 0 ? -1 : 1);
	_position = vec;
	_normalizedPosition = D3DXVECTOR2(parentPos.x + vec.x* parentScale.x* direction.x, parentPos.y  + vec.y * parentScale.y* direction.y);

	for (auto child : _childList)
	{
		child->_normalizedPosition = vec;
		child->Position(child->_position);
	}
}

D3DXVECTOR2 Node::Position()
{
	/*D3DXVECTOR2 parentPos = D3DXVECTOR2(0, 0);
	if (_parent != nullptr)
		parentPos = _parent->Position();
	auto result = _position- parentPos;*/
	return _position;
}

void Node::AnchorPosition(float x, float y)
{
	AnchorPosition(D3DXVECTOR2(x, y));
}

void Node::AnchorPosition(D3DXVECTOR2 vec)
{
	_anchorPoint = vec;
}

void Node::Scale(float x, float y)
{
	Scale(D3DXVECTOR2(x, y));
}

void Node::Scale(D3DXVECTOR2 vec)
{
	D3DXVECTOR2 parentScale = D3DXVECTOR2(1, 1);
	if (_parent != nullptr)
		parentScale = _parent->_normalizedScale;

	_scale = vec;
	_normalizedScale = D3DXVECTOR2(parentScale.x*vec.x, parentScale.y*vec.y);
	Position(_position);

	for (auto child : _childList)
	{
		child->_normalizedScale = vec;
		child->Scale(child->_scale);
	}
}

D3DXVECTOR2 Node::Scale()
{
	/*D3DXVECTOR2 parentScale = D3DXVECTOR2(1, 1);
	if (_parent != nullptr)
		parentScale = _parent->Scale();
	auto result = D3DXVECTOR2(_scale.x/parentScale.x, _scale.y/parentScale.y);*/
	return _scale;
}

void Node::Rotation(float x, float y, float z)
{
	Rotation(D3DXVECTOR3(x, y, z));
}

void Node::Rotation(D3DXVECTOR3 vec)
{
	D3DXVECTOR3 parentRot = D3DXVECTOR3(0, 0,0);
	//D3DXVECTOR2 parentPos = D3DXVECTOR2(0, 0);
	if (_parent != nullptr)
	{
		parentRot = _parent->_normalizedRotation;
		//parentPos = _parent->_normalizedPosition;
	}
		

	_rotation = vec;
	_normalizedRotation = D3DXVECTOR3(parentRot.x+vec.x, parentRot.y+vec.y , parentRot.z+vec.z);
	Position(_position);

	for (auto child : _childList)
	{
		child->_normalizedRotation = vec;
		child->Rotation(child->_rotation);
	}
}

D3DXVECTOR3 Node::Rotation()
{
	/*D3DXVECTOR3 parentRot = D3DXVECTOR3(0, 0, 0);
	if (_parent != nullptr)
		parentRot = _parent->Rotation();
	auto result = _rotation - parentRot;*/
	return _rotation;
}

void Node::RotationDegree(float x, float y, float z)
{
	RotationDegree(D3DXVECTOR3(x, y, z));
}

void Node::RotationDegree(D3DXVECTOR3 vec)
{
	D3DXVECTOR3 temp = Rotation();
	temp.x = Math::ToRadian(vec.x);
	temp.y = Math::ToRadian(vec.y);
	temp.z = Math::ToRadian(vec.z);


	Rotation(temp);
}

D3DXVECTOR3 Node::RotationDegree()
{
	D3DXVECTOR3 vec;
	vec.x = Math::ToDegree(Rotation().x);
	vec.y = Math::ToDegree(Rotation().y);
	vec.z = Math::ToDegree(Rotation().z);

	return vec;
}

Node* Node::autorelease()
{
	Director::getInstance()->AddReleaseList(this);
	return this;
}

void Node::release()
{
	//assert(_referenceCount > 0);
	_referenceCount--;

	if (_referenceCount == 0)
	{
		delete this;
	}
}

Node* Node::getChildByTag(int tag) const
{
	assert(tag != -1);

	for (const auto child : _childList)
	{
		if (child && child->_tag == tag)
			return child;
	}
	return nullptr;
}

void Node::AddEvent(const std::function<void()>& func, float timer)
{
	auto event = pair<std::function<void()>, float>(func, timer);
	eventList.push_back(event);
}

void Node::_setLocalZOrder(std::int32_t z)
{
	_localZOrder = z;
}


void Node::retain()
{
	_referenceCount++;
}

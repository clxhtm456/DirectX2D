#pragma once

class Node
{
public:
	static Node* Create();
	virtual bool Init();

	virtual void PostUpdate();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();
	virtual void Draw();
	virtual void AddChild(Node* child, int _localZOrder);
	virtual void AddChild(Node* child);
	virtual void removeFromParent();
	virtual void removeChild(Node* child, bool cleanup = true);
	virtual void Reset();

	virtual void OnEnter();
	virtual void OnExit();

	bool GetRunning() { return _running; }
	virtual void SetRunning(bool val) { _running = val; }

	void SetParent(Node* parent);
	void detachChild(Node *child, SSIZE_T index, bool doCleanup);

	void sortAllChildren();
	void RemoveSelf();
	void SetInvisible();
	void SetVisible();
	bool IsVisible();

	Node* Parent();

	virtual void Position(float x, float y);
	virtual void Position(D3DXVECTOR2 vec);
	D3DXVECTOR2 Position();

	D3DXVECTOR2 NormalizedPosition() {return _normalizedPosition;}

	virtual void Scale(float x, float y);
	virtual void Scale(D3DXVECTOR2 vec);
	D3DXVECTOR2 Scale();

	virtual void AnchorPosition(float x, float y);
	virtual void AnchorPosition(D3DXVECTOR2 vec);
	virtual D3DXVECTOR2 AnchorPosition() { return _anchorPoint; }

	virtual void Rotation(float x, float y, float z);
	virtual void Rotation(D3DXVECTOR3 vec);
	D3DXVECTOR3 Rotation();

	void RotationDegree(float x, float y, float z);
	void RotationDegree(D3DXVECTOR3 vec);
	D3DXVECTOR3 RotationDegree();

	Node* autorelease();
	void retain();
	void release();

	int Tag() { return _tag;}
	void Tag(int val) { _tag = val; }

	virtual Node* getChildByTag(int tag) const;

	void SetVisible(bool val) { _visible = val; }

	void AddEvent(const std::function<void()>& func, float timer);

	virtual const vector<Node*>& getChildren() const { return _childList; }

	virtual std::int32_t getLocalZOrder() const { return _localZOrder; }
	virtual void _setLocalZOrder(std::int32_t z);
private:
	vector<Node*> _childList;
	class Director* _director;

	vector<pair<std::function<void()>, float>> eventList;

	bool _reorderChildDirty;

	union {
		struct {
			std::uint32_t _orderOfArrival;
			std::int32_t _localZOrder;
		};
		std::int64_t _localZOrder$Arrival;
	};

	Node* _parent;

	unsigned int _referenceCount;

protected:
	Node();
	virtual ~Node();

	bool _running;
	bool _visible;

	int _tag;

	D3DXVECTOR2 _normalizedPosition;
	D3DXVECTOR3 _normalizedRotation;
	D3DXVECTOR2 _normalizedScale;

	D3DXVECTOR2 _position;
	D3DXVECTOR2 _anchorPoint;
	D3DXVECTOR2 _scale;
	D3DXVECTOR3 _rotation;
};
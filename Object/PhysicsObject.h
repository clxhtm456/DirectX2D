#ifndef __PHYSICS_OBJECT_H__
#define __PHYSICS_OBJECT_H__

#include "PhysicsBox.h"
#include "Renders/Rect.h"

typedef enum ColDirection
{
	UP,
	DOWN,
	RIGHT,
	LEFT
};

class PhysicsObject : public Node
{
public:
	bool Init();
	void Reset();
	virtual ~PhysicsObject();

	D3DXVECTOR2 BottomVector() { return bottomVector; }

	void LineBottomCollision(bool val);

	bool OnGround() { return bGround; }
	bool isLanding() { return (velocity <= 0); }

	void LineLeftPoint(D3DXVECTOR2 val) { linePoint.first = val; }
	void LineRightPoint(D3DXVECTOR2 val) { linePoint.second = val; }

	bool LeftCollision() { return leftCollision; }
	bool RightCollision() { return rightCollision; }
	void LeftCollision(bool val) { leftCollision = val; }
	void RightCollision(bool val) { rightCollision = val; }
	bool TopCollision() { return topCollision; }
	bool BottomCollision() { return bottomCollision; }
	void TopCollision(bool val) { topCollision = val; }
	void BottomCollision(bool val) { bottomCollision = val; }

	bool ObjLeftCollision() { return objleftCollision; }
	bool ObjRightCollision() { return objrightCollision; }
	void ObjLeftCollision(bool val) { objleftCollision = val; }
	void ObjRightCollision(bool val) { objrightCollision = val; }
	bool ObjTopCollision() { return objtopCollision; }
	bool ObjBottomCollision() { return objbottomCollision; }
	void ObjTopCollision(bool val) { objtopCollision = val; }
	void ObjBottomCollision(bool val) { objbottomCollision = val; }

	int Flag() { return _flag; }
	void SetFlagZero() { _flag = 0; }
	bool HasFlag(int val) { return _flag &= val; }
	virtual void Flag(int val) { AddFlag(val); }
	virtual void AddFlag(int val) { _flag |= val; }
	virtual void DecFlag(int val) { _flag &= ~val; }
	//void Flag(int val) { _flag = val; }

	void PhysicsActive(bool val) { bActive = val; }

	bool Aabb(D3DXVECTOR2 targetPosition);
	bool Aabb(PhysicsObject* b, OUT ColDirection& direction);

	bool CheckCollision(PhysicsObject* b);
	void EnterCollision(PhysicsObject* b, ColDirection direction);
	void ExitCollision(PhysicsObject* b);
	void ExitCollisionAll();

	virtual void EnterPhysicsCollision(PhysicsObject* b, ColDirection direction) = 0;
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) = 0;
	virtual void ExitPhysicsCollision(PhysicsObject* b) = 0;

	
	void PostUpdate();
	void Update();
	void LateUpdate();
	void Render();
	void DrawBoundingBox();

	void SetKinematic(bool val) { isKinematic = val; }
	bool GetKinematic() { return isKinematic; }
private:
	int _flag;
public:
	static vector<PhysicsObject*> physicsObjectList;
	//static vector<PhysicsObject*> destroyNode;

	static void Destroy(PhysicsObject* node);
//------------------------------------
//Collider
//------------------------------------
protected:
	
	bool bActive;

	Rect* rect;
	PhysicsBox* collider;
	ID3D11Buffer* boundVertexBuffer;
	D3DXMATRIX world;
	Shader* boundShader;

	struct BoundVertex
	{
		D3DXVECTOR3 Position;
	};
public:
	PhysicsBox* GetCollider()const { return collider; }
	float ColliderLeftPoint(); 
	float ColliderRightPoint();
	float ColliderTopPoint();
	float ColliderBottomPoint();
//------------------------------------
protected:
	D3DXVECTOR2 moveAccel;
	D3DXVECTOR3 rotation;

	D3DXVECTOR2 bottomVector;

	D3DXVECTOR2 currentPosition;

	float moveSpeed;
	float rollSpeed;

	float accel;
	float velocity;
	float gravity;

	bool bGround;
	bool isKinematic;
protected:
	//오브젝트 충돌
	vector<PhysicsObject*> collisionList;

protected:
	//라인충돌
	pair<D3DXVECTOR2, D3DXVECTOR2> linePoint;
	bool bLineBottonCollision;

	bool leftCollision;
	bool rightCollision;
	bool topCollision;
	bool bottomCollision;

	bool objleftCollision;
	bool objrightCollision;
	bool objtopCollision;
	bool objbottomCollision;

	D3DXVECTOR3 degree;
private:

};

#endif
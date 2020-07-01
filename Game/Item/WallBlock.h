#pragma once

#include "Object/EventBox.h"

class WallBlock : public PhysicsObject
{
public:
	static WallBlock* Create();
	bool Init();

	~WallBlock();

	void Update();

	void LeftCol(bool val) { leftcol = val; }
	void RightCol(bool val) { rightcol = val; }
	void TopCol(bool val) { topcol = val; }
	void BottomCol(bool val) { bottomcol = val; }
protected:

	// PhysicsObject��(��) ���� ��ӵ�
	virtual void EnterPhysicsCollision(PhysicsObject * b, ColDirection direction) override;

	// PhysicsObject��(��) ���� ��ӵ�
	virtual void ExitPhysicsCollision(PhysicsObject * b) override;
private:
	bool leftcol;
	bool rightcol;
	bool topcol;
	bool bottomcol;

	D3DXVECTOR2 recentPos;

	vector<pair<PhysicsObject*,ColDirection>> directionList;


	// PhysicsObject��(��) ���� ��ӵ�
	virtual void OnPhysicsCollision(PhysicsObject* b, ColDirection direction) override;

};
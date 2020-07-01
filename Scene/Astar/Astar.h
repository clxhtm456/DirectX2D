#pragma once

#include "Renders/Rect.h"

#define WidthNode	12
#define HeightNode	15


enum class NodeType
{
	None,
	Start,
	Goal,
	Wall
};

struct NodeDesc
{
	//시작 , 골
	bool start;
	bool goal;
	bool wall;

	//비용
	float f;
	float g;
	float h;

	bool bClose;
	D3DXVECTOR2 parent;
};

class Astar
{
	//color 0xa,r,g,b
public:
	Astar(D3DXVECTOR2 position, D3DXVECTOR2 scale = D3DXVECTOR2(1,1), D3DXCOLOR color = D3DXCOLOR(0xff555566));
	~Astar();

	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	UINT FindH(int startX, int startY, int goalX,int goalY);
	void Reset();

public:
	void RenderText();
	void SetStart();
	void SetGoal();
	void SetWall();

	void FindPath();


public:
	void SetMouseCursor(D3DXVECTOR2 ptMouse) { this->ptMouse = ptMouse; }
	void SetStartPos(D3DXVECTOR2 startPos) { this->startPos = startPos; }
	void SetGoalPos(D3DXVECTOR2 goalPos) { this->goalPos = goalPos; }
	void SetCallBack(const function<void(vector<POINT>)>& _callback)
	{
		callback = _callback;
	}

	void ReadyToStart(bool val) { bReadyToMove = val; }
	bool ReadyToStart() { return bReadyToMove; }

	D3DXVECTOR2 Position();
	vector<POINT> WayPoint();

private:
	NodeType nodeType;

	pair<Rect*, NodeDesc> nodes[HeightNode][WidthNode];
	D3DXCOLOR color;

	int startX, startY;
	int goalX, goalY;

	vector<POINT> way;

	//마우스
	D3DXVECTOR2 ptMouse;

	D3DXVECTOR2 startPos;
	D3DXVECTOR2 goalPos;

	bool bReadyToMove;

	function<void(vector<POINT>)> callback;
};
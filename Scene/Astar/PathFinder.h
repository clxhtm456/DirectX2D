#pragma once

#define WidthNode 12
#define HeightNode 15

enum class NodeType { None, Start, Goal, Wall };

struct NodeDesc
{
	//시작, 골
	bool start;
	bool goal;
	bool wall;

	//비용
	float f;
	float g;
	float h;

	bool close;
	D3DXVECTOR2 parent;
};

class PathFinder
{
private:
	NodeType nodeType;

	pair<Rect*, NodeDesc> nodes[WidthNode][HeightNode];
	D3DXCOLOR color;

	int startX, startY;
	int goalX, goalY;
	vector<POINT> way;

	//마우스용
	D3DXVECTOR2 ptMouse;

	//플레이어, 몬스터 위치
	D3DXVECTOR2 startPos;
	D3DXVECTOR2 goalPos;

	bool bReadyToMove;

public:
	void RenderText();

	void SetStart();
	void SetGoal();
	void SetWall();

	void FindPath();

public:
	PathFinder(D3DXVECTOR2 position, D3DXVECTOR2 scale = D3DXVECTOR2(1, 1), D3DXCOLOR color = D3DXCOLOR(0xff555566));
	~PathFinder();

	void Udpate(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();

	UINT FindH(int startX, int startY, int goalX, int goalY);
	void Reset();

	void GetMouseCursor(D3DXVECTOR2 ptMouse) { this->ptMouse = ptMouse; }

	void GetStartPos(D3DXVECTOR2 start) { startPos = start; }
	void GetGoalPos(D3DXVECTOR2 goal) { goalPos = goal; }

	void ReadyToStart(bool val) { bReadyToMove = val; }
	bool ReadyToStart() { return bReadyToMove; }

	D3DXVECTOR2 Position();



};
#include "stdafx.h"
#include "Astar.h"

#define Infinite	INT_MAX

#define Nodes		nodes[y][x]
#define EveryNode(__value__);	\
for (int y = 0; y < HeightNode; y++)\
{\
	for (int x = 0; x < WidthNode; x++)\
	{\
		__value__\
	}\
}\

Astar::Astar(D3DXVECTOR2 position, D3DXVECTOR2 scale, D3DXCOLOR color):
	nodeType(NodeType::None),
	color(color),
	startX(0),
	startY(0),
	goalX(0),
	goalY(0),
	ptMouse(0,0),
	startPos(0,0),
	goalPos(0,0),
	bReadyToMove(false),
	callback(NULL)
{
	wstring shaderFile = Shaders + L"Rect.fx";

	float gridX = Width / scale.x;
	float gridY = Height / scale.y;

	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{
			float positionX = x * scale.x - Width * 0.5f;
			float positionY = y * scale.y - Height * 0.5f;

			Nodes.first = new Rect(shaderFile, D3DXVECTOR2(positionX, positionY), scale);
			Nodes.second.start = false;
			Nodes.second.goal = false;
			Nodes.second.wall = false;
			Nodes.second.f = Infinite;
			Nodes.second.g = Infinite;
			Nodes.second.h = Infinite;
			Nodes.second.bClose = false;
			Nodes.second.parent.x = Infinite;
			Nodes.second.parent.y = Infinite;
		}
	}
}

Astar::~Astar()
{
	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{
			SAFE_DELETE(Nodes.first);
		}
	}
}

void Astar::Update(D3DXMATRIX& V, D3DXMATRIX& P)
{
	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{
			Nodes.first->Update(V, P);
		}
	}

	if (bReadyToMove == true)
	{
		nodeType = NodeType::Start;
		bReadyToMove = false;
	}
	else if (bReadyToMove == false)
		nodeType = NodeType::None;

	//마우스 키 세팅
	if (Mouse->Down(1))
		nodeType = NodeType::Goal;
	else if (Mouse->Up(1))
		nodeType = NodeType::None;

	if (Mouse->Press(0))
		nodeType = NodeType::Wall;
	else if (Mouse->Up(1))
		nodeType = NodeType::None;

	//재시작
	if (Key->Down(VK_ESCAPE)) Reset();

	switch (nodeType)
	{
		case NodeType::Start: break;
		case NodeType::Goal: SetGoal(); SetStart(); break;
		case NodeType::Wall: SetWall(); break;
	}

	if (Key->Down(VK_RETURN))
		FindPath();
		
}

void Astar::Render()
{
	ImGui::Text("Node");
	ImGui::LabelText("startPos", "%0.2f, %0.2f", startPos.x, startPos.y);
	ImGui::LabelText("goalPos", "%0.2f, %0.2f", goalPos.x, goalPos.y);
	ImGui::LabelText("Node Type", "%d", nodeType);

	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{

			//시작점
			if (Nodes.second.start)
				Nodes.first->Color(1, 1, 0);//노랑
			//도착지
			else if (Nodes.second.goal)
				Nodes.first->Color(0, 1, 0);//초록
			//벽
			else if (Nodes.second.wall)
				Nodes.first->Color(1, 0, 0);//빨강
			//닫힘
			else if (Nodes.second.bClose)
				Nodes.first->Color(0, 0, 1);//파랑
		}
	}

	//길
	for (UINT i = 0; i < way.size(); i++)
	{
		nodes[way[i].y][way[i].x].first->Color(1, 1, 1);
	}

	RenderText();

	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{
			Nodes.first->Render();
		}
	}

	for (UINT i = 0; i < way.size(); i++)
	{
		D3DXVECTOR2 position = nodes[way[i].y][way[i].x].first->Position();
		ImGui::LabelText("Way Vector(node)", "%0.2f, %0.2f", position.x, position.y);
	}

	
}

UINT Astar::FindH(int startX, int startY, int goalX, int goalY)
{
	int distanceX;
	int distanceY;
	int distance;

	distanceX = abs(startX - goalX);
	distanceY = abs(startY - goalY);

	return (distanceX + distanceY) * 10;
}

void Astar::Reset()
{
	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{
			Nodes.second.start = false;
			Nodes.second.goal = false;
			Nodes.second.wall = false;
			Nodes.second.f = Infinite;
			Nodes.second.g = Infinite;
			Nodes.second.h = Infinite;
			Nodes.second.bClose = false;
			Nodes.second.parent.x = Infinite;
			Nodes.second.parent.y = Infinite;

			Nodes.first->Color(0, 0, 0);
		}
	}
	way.clear();
	nodeType = NodeType::None;
}

void Astar::RenderText()
{
	DirectWrite::GetDC()->BeginDraw();
	{
		wstring text = L"RB 시작 + 골//LB 장애물//[ENTER] 길찾기//[ESC] 리셋";

		RECT rt = { 10,80,600,200 };
		DirectWrite::RenderText(text, rt);

		rt.top += 20;
		rt.bottom += 20;
		text = L"출발지(YELLOW), 목적지(GREEN), 벽(RED), 닫힘(BLUE), 최종길(WHITE)";
		DirectWrite::RenderText(text, rt);
	}
	DirectWrite::GetDC()->EndDraw();
}

void Astar::SetStart()
{
	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{
			if (Rect::PtInRect(Nodes.first, startPos))
			{
				Nodes.second.start = true;
				Nodes.second.g = 0;
				startX = x;
				startY = y;
			}
			else
			{
				Nodes.second.start = false;
			}
		}
	}
}

void Astar::SetGoal()
{
	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{
			if (Rect::PtInRect(Nodes.first, ptMouse))
			{
				Nodes.second.goal = true;
				goalX = x;
				goalY = y;
			}
			else
			{
				Nodes.second.goal = false;
			}
		}
	}
}

void Astar::SetWall()
{
	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{
			if (Rect::PtInRect(Nodes.first, ptMouse))
			{
				Nodes.second.wall = true;
			}
		}
	}
}

void Astar::FindPath()
{
	for (int y = 0; y < HeightNode; y++)
	{
		for (int x = 0; x < WidthNode; x++)
		{
			Nodes.second.h = FindH(y, x, goalY, goalX);
		}
	}

	//Min F 검사 시작
	int count = 0;

	while (true)
	{
		if (count == HeightNode * WidthNode)
			break;
		count++;

		//F = G+H
		for (int y = 0; y < HeightNode; y++)
		{
			for (int x = 0; x < WidthNode; x++)
			{
				if (Nodes.second.g != Infinite)
				{
					Nodes.second.f = Nodes.second.g + Nodes.second.h;
				}
			}
		}

		//f 최소값 계산

		//인접노드 검색 및 인덱스 저장
		POINT minPoint;
		int count2 = 0;
		for (int y = 0; y < HeightNode; y++)
		{
			for (int x = 0; x < WidthNode; x++)
			{
				//검사대상 선별
				if (Nodes.second.f&&
					Nodes.second.wall == false &&
					Nodes.second.bClose == false)
				{
					minPoint.x = x;
					minPoint.y = y;
					break;
				}
				count2++;
			}
		}

		if (count2 == HeightNode * WidthNode)
		{
			break;
		}

		//실질적으로 minF를 구하는 곳!
		for (UINT y = 0; y < HeightNode; y++)
		{
			for (UINT x = 0; x < WidthNode; x++)
			{
				//꺼내놓은 인덱스가 다른 노드보다 F 값이 크면
				if (nodes[minPoint.y][minPoint.x].second.f > Nodes.second.f&&
					Nodes.second.wall == false &&
					Nodes.second.bClose == false)
				{
					//현재 노드를 인덱스에 다시 저장
					minPoint.y = y;
					minPoint.x = x;
				}
			}
		}
			//왼쪽
			if (minPoint.x != 0)
			{
				if (nodes[minPoint.y][minPoint.x - 1].second.g > 10 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y][minPoint.x - 1].second.g = 10 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y][minPoint.x - 1].second.parent.x = minPoint.x;
					nodes[minPoint.y][minPoint.x - 1].second.parent.y = minPoint.y;
				}
			}

			//오른쪽
			if (minPoint.x != WidthNode - 1)
			{
				if (nodes[minPoint.y][minPoint.x + 1].second.g > 10 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y][minPoint.x + 1].second.g = 10 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y][minPoint.x + 1].second.parent.x = minPoint.x;
					nodes[minPoint.y][minPoint.x + 1].second.parent.y = minPoint.y;
				}
			}

			//위
			if (minPoint.y != 0)
			{
				if (nodes[minPoint.y - 1][minPoint.x].second.g > 10 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y - 1][minPoint.x].second.g = 10 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y - 1][minPoint.x].second.parent.x = minPoint.x;
					nodes[minPoint.y - 1][minPoint.x].second.parent.y = minPoint.y;
				}
			}

			//아래
			if (minPoint.y != HeightNode - 1)
			{
				if (nodes[minPoint.y + 1][minPoint.x].second.g > 10 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y + 1][minPoint.x].second.g = 10 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y + 1][minPoint.x].second.parent.x = minPoint.x;
					nodes[minPoint.y + 1][minPoint.x].second.parent.y = minPoint.y;
				}
			}

			//좌상
			if (minPoint.x != 0 && minPoint.y != 0)
			{
				if (nodes[minPoint.y - 1][minPoint.x].second.wall == true &&
					nodes[minPoint.y][minPoint.x - 1].second.wall == true)
				{
					//비웠을 때랑 아닐 때 테스트
				}

				else if (nodes[minPoint.y - 1][minPoint.x - 1].second.g > 14 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y - 1][minPoint.x - 1].second.g = 14 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y - 1][minPoint.x - 1].second.parent.x = minPoint.x;
					nodes[minPoint.y - 1][minPoint.x - 1].second.parent.y = minPoint.y;
				}
			}

			//좌하
			if (minPoint.x != 0 && minPoint.y != HeightNode - 1)
			{
				if (nodes[minPoint.y + 1][minPoint.x].second.wall == true &&
					nodes[minPoint.y][minPoint.x - 1].second.wall == true)
				{
					//비웠을 때랑 아닐 때 테스트
				}

				else if (nodes[minPoint.y + 1][minPoint.x - 1].second.g > 14 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y + 1][minPoint.x - 1].second.g = 14 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y + 1][minPoint.x - 1].second.parent.x = minPoint.x;
					nodes[minPoint.y + 1][minPoint.x - 1].second.parent.y = minPoint.y;
				}
			}

			//우상
			if (minPoint.x != WidthNode - 1 && minPoint.y != 0)
			{
				if (nodes[minPoint.y - 1][minPoint.x].second.wall == true &&
					nodes[minPoint.y][minPoint.x + 1].second.wall == true)
				{
					//비웠을 때랑 아닐 때 테스트
				}

				else if (nodes[minPoint.y - 1][minPoint.x + 1].second.g > 14 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y - 1][minPoint.x + 1].second.g = 14 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y - 1][minPoint.x + 1].second.parent.x = minPoint.x;
					nodes[minPoint.y - 1][minPoint.x + 1].second.parent.y = minPoint.y;
				}
			}

			//우하
			if (minPoint.x != WidthNode - 1 && minPoint.y != HeightNode - 1)
			{
				if (nodes[minPoint.y + 1][minPoint.x].second.wall == true &&
					nodes[minPoint.y][minPoint.x + 1].second.wall == true)
				{
					//비웠을 때랑 아닐 때 테스트
				}

				else if (nodes[minPoint.y + 1][minPoint.x + 1].second.g > 14 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y + 1][minPoint.x + 1].second.g = 14 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y + 1][minPoint.x + 1].second.parent.x = minPoint.x;
					nodes[minPoint.y + 1][minPoint.x + 1].second.parent.y = minPoint.y;
				}
			}

			nodes[minPoint.y][minPoint.x].second.bClose = true;

		//길 그리기
		if (nodes[minPoint.y][minPoint.x].second.goal == true)
		{
			POINT tempPoint;//MinF 의 현재 위치 저장
			tempPoint.y = minPoint.y;
			tempPoint.x = minPoint.x;
			
			POINT tempPoint2;//MinF의 부모인덱스 저장

			while (true)
			{
				//부모 저장
				tempPoint2.y = nodes[tempPoint.y][tempPoint.x].second.parent.y;
				tempPoint2.x = nodes[tempPoint.y][tempPoint.x].second.parent.x;

				//minF -> 부모로 변경
				tempPoint.y = tempPoint2.y;
				tempPoint.x = tempPoint2.x;

				way.push_back(tempPoint);

				//시작점에 tempPoint가 도달시 반복문 종료
				if (nodes[tempPoint.y][tempPoint.x].second.start == true)
				{
					reverse(way.begin(), way.end());
					if (callback)
					{
						callback(way);
					}
					break;
				}
				//예외처리
				if (tempPoint.y == Infinite)
				{
					Reset();
					break;
				}

				if (way.size() == WidthNode * HeightNode)
				{
					//Reset();
					break;
				}
			}
			break;
		}


	}
}

D3DXVECTOR2 Astar::Position()
{
	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{
			if (Rect::PtInRect(Nodes.first, ptMouse))
			{
				return Nodes.first->Position();
			}
		}
	}

	return D3DXVECTOR2(0, 0);
}

vector<POINT> Astar::WayPoint()
{
	if (way.size() == 0)
		assert(false);
	return way;
}

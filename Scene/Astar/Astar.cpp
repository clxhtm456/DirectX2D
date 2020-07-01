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

	//���콺 Ű ����
	if (Mouse->Down(1))
		nodeType = NodeType::Goal;
	else if (Mouse->Up(1))
		nodeType = NodeType::None;

	if (Mouse->Press(0))
		nodeType = NodeType::Wall;
	else if (Mouse->Up(1))
		nodeType = NodeType::None;

	//�����
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

			//������
			if (Nodes.second.start)
				Nodes.first->Color(1, 1, 0);//���
			//������
			else if (Nodes.second.goal)
				Nodes.first->Color(0, 1, 0);//�ʷ�
			//��
			else if (Nodes.second.wall)
				Nodes.first->Color(1, 0, 0);//����
			//����
			else if (Nodes.second.bClose)
				Nodes.first->Color(0, 0, 1);//�Ķ�
		}
	}

	//��
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
		wstring text = L"RB ���� + ��//LB ��ֹ�//[ENTER] ��ã��//[ESC] ����";

		RECT rt = { 10,80,600,200 };
		DirectWrite::RenderText(text, rt);

		rt.top += 20;
		rt.bottom += 20;
		text = L"�����(YELLOW), ������(GREEN), ��(RED), ����(BLUE), ������(WHITE)";
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

	//Min F �˻� ����
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

		//f �ּҰ� ���

		//������� �˻� �� �ε��� ����
		POINT minPoint;
		int count2 = 0;
		for (int y = 0; y < HeightNode; y++)
		{
			for (int x = 0; x < WidthNode; x++)
			{
				//�˻��� ����
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

		//���������� minF�� ���ϴ� ��!
		for (UINT y = 0; y < HeightNode; y++)
		{
			for (UINT x = 0; x < WidthNode; x++)
			{
				//�������� �ε����� �ٸ� ��庸�� F ���� ũ��
				if (nodes[minPoint.y][minPoint.x].second.f > Nodes.second.f&&
					Nodes.second.wall == false &&
					Nodes.second.bClose == false)
				{
					//���� ��带 �ε����� �ٽ� ����
					minPoint.y = y;
					minPoint.x = x;
				}
			}
		}
			//����
			if (minPoint.x != 0)
			{
				if (nodes[minPoint.y][minPoint.x - 1].second.g > 10 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y][minPoint.x - 1].second.g = 10 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y][minPoint.x - 1].second.parent.x = minPoint.x;
					nodes[minPoint.y][minPoint.x - 1].second.parent.y = minPoint.y;
				}
			}

			//������
			if (minPoint.x != WidthNode - 1)
			{
				if (nodes[minPoint.y][minPoint.x + 1].second.g > 10 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y][minPoint.x + 1].second.g = 10 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y][minPoint.x + 1].second.parent.x = minPoint.x;
					nodes[minPoint.y][minPoint.x + 1].second.parent.y = minPoint.y;
				}
			}

			//��
			if (minPoint.y != 0)
			{
				if (nodes[minPoint.y - 1][minPoint.x].second.g > 10 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y - 1][minPoint.x].second.g = 10 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y - 1][minPoint.x].second.parent.x = minPoint.x;
					nodes[minPoint.y - 1][minPoint.x].second.parent.y = minPoint.y;
				}
			}

			//�Ʒ�
			if (minPoint.y != HeightNode - 1)
			{
				if (nodes[minPoint.y + 1][minPoint.x].second.g > 10 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y + 1][minPoint.x].second.g = 10 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y + 1][minPoint.x].second.parent.x = minPoint.x;
					nodes[minPoint.y + 1][minPoint.x].second.parent.y = minPoint.y;
				}
			}

			//�»�
			if (minPoint.x != 0 && minPoint.y != 0)
			{
				if (nodes[minPoint.y - 1][minPoint.x].second.wall == true &&
					nodes[minPoint.y][minPoint.x - 1].second.wall == true)
				{
					//����� ���� �ƴ� �� �׽�Ʈ
				}

				else if (nodes[minPoint.y - 1][minPoint.x - 1].second.g > 14 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y - 1][minPoint.x - 1].second.g = 14 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y - 1][minPoint.x - 1].second.parent.x = minPoint.x;
					nodes[minPoint.y - 1][minPoint.x - 1].second.parent.y = minPoint.y;
				}
			}

			//����
			if (minPoint.x != 0 && minPoint.y != HeightNode - 1)
			{
				if (nodes[minPoint.y + 1][minPoint.x].second.wall == true &&
					nodes[minPoint.y][minPoint.x - 1].second.wall == true)
				{
					//����� ���� �ƴ� �� �׽�Ʈ
				}

				else if (nodes[minPoint.y + 1][minPoint.x - 1].second.g > 14 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y + 1][minPoint.x - 1].second.g = 14 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y + 1][minPoint.x - 1].second.parent.x = minPoint.x;
					nodes[minPoint.y + 1][minPoint.x - 1].second.parent.y = minPoint.y;
				}
			}

			//���
			if (minPoint.x != WidthNode - 1 && minPoint.y != 0)
			{
				if (nodes[minPoint.y - 1][minPoint.x].second.wall == true &&
					nodes[minPoint.y][minPoint.x + 1].second.wall == true)
				{
					//����� ���� �ƴ� �� �׽�Ʈ
				}

				else if (nodes[minPoint.y - 1][minPoint.x + 1].second.g > 14 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y - 1][minPoint.x + 1].second.g = 14 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y - 1][minPoint.x + 1].second.parent.x = minPoint.x;
					nodes[minPoint.y - 1][minPoint.x + 1].second.parent.y = minPoint.y;
				}
			}

			//����
			if (minPoint.x != WidthNode - 1 && minPoint.y != HeightNode - 1)
			{
				if (nodes[minPoint.y + 1][minPoint.x].second.wall == true &&
					nodes[minPoint.y][minPoint.x + 1].second.wall == true)
				{
					//����� ���� �ƴ� �� �׽�Ʈ
				}

				else if (nodes[minPoint.y + 1][minPoint.x + 1].second.g > 14 + nodes[minPoint.y][minPoint.x].second.g)
				{
					nodes[minPoint.y + 1][minPoint.x + 1].second.g = 14 + nodes[minPoint.y][minPoint.x].second.g;
					nodes[minPoint.y + 1][minPoint.x + 1].second.parent.x = minPoint.x;
					nodes[minPoint.y + 1][minPoint.x + 1].second.parent.y = minPoint.y;
				}
			}

			nodes[minPoint.y][minPoint.x].second.bClose = true;

		//�� �׸���
		if (nodes[minPoint.y][minPoint.x].second.goal == true)
		{
			POINT tempPoint;//MinF �� ���� ��ġ ����
			tempPoint.y = minPoint.y;
			tempPoint.x = minPoint.x;
			
			POINT tempPoint2;//MinF�� �θ��ε��� ����

			while (true)
			{
				//�θ� ����
				tempPoint2.y = nodes[tempPoint.y][tempPoint.x].second.parent.y;
				tempPoint2.x = nodes[tempPoint.y][tempPoint.x].second.parent.x;

				//minF -> �θ�� ����
				tempPoint.y = tempPoint2.y;
				tempPoint.x = tempPoint2.x;

				way.push_back(tempPoint);

				//�������� tempPoint�� ���޽� �ݺ��� ����
				if (nodes[tempPoint.y][tempPoint.x].second.start == true)
				{
					reverse(way.begin(), way.end());
					if (callback)
					{
						callback(way);
					}
					break;
				}
				//����ó��
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

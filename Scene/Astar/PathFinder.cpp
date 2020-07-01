#include "stdafx.h"
#include "PathFinder.h"

//RenderText, Position

#define Inf INT_MAX
#define HeightNode 12
#define WidthNode 15

#define Nodes nodes[y][x]

PathFinder::PathFinder(D3DXVECTOR2 position, D3DXVECTOR2 scale, D3DXCOLOR color)
	: nodeType(NodeType::None), color(color)
	, startX(0), startY(0), goalX(0), goalY(0)
	, ptMouse(0, 0)
	, startPos(0, 0), goalPos(0, 0), bReadyToMove(false)
{
	wstring shaderFile = Shaders + L"Rect.fx";

	float gridX = Width / scale.x;
	float gridY = Height / scale.y;

	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{
			float positionX = x * scale.x - Width * 0.5f;
			float positionY = y * scale.y - Height * 0.5f;

			Nodes.first = new Rect(shaderFile, D3DXVECTOR2(positionX, positionY), scale);
			Nodes.second.start = false;
			Nodes.second.goal = false;
			Nodes.second.wall = false;
			Nodes.second.f = Inf;
			Nodes.second.g = Inf;
			Nodes.second.h = Inf;
			Nodes.second.close = false;
			Nodes.second.parent.x = Inf;
			Nodes.second.parent.y = Inf;
		}
	}
}

PathFinder::~PathFinder()
{
	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{
			SAFE_DELETE(Nodes.first);
		}
	}

}

void PathFinder::RenderText()
{
	DirectWrite::GetDC()->BeginDraw();
	{
		wstring text = L"RB ���� + �� // LB : ��ֹ� // [RETURN] ��ã�� // [ESC] ����";

		RECT rt = { 10, 80, 600, 200 };
		DirectWrite::RenderText(text, rt);

		rt.top += 20;
		rt.bottom += 20;
		text = L"�����(YELLOW), ������(GREEN), ��(RED), ����(BLUE), ������(WHITE)";
		DirectWrite::RenderText(text, rt);

	}
	DirectWrite::GetDC()->EndDraw();
}

void PathFinder::SetStart()
{
	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
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

void PathFinder::SetGoal()
{
	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
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

void PathFinder::SetWall()
{
	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{
			if (Rect::PtInRect(Nodes.first, ptMouse))
			{
				Nodes.second.wall = true;
			}
		}
	}

}

void PathFinder::FindPath()
{
	//H (���� ���)
	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{
			Nodes.second.h = FindH(y, x, goalY, goalX);
		}
	}

	//Min F �˻� ����
	int count = 0;

	while (true)
	{
		//�˻� ȸ���� ��ü ��󺸴� ������ �̻�(�ݺ��� Ż��)
		if (count = HeightNode * WidthNode)
		{
			break;
		}
		count++;

		//F = G + H
		for (UINT y = 0; y < HeightNode; y++)
		{
			for (UINT x = 0; x < WidthNode; x++)
			{
				if (Nodes.second.g != Inf)
				{
					Nodes.second.f = Nodes.second.h + Nodes.second.g;
				}
			}
		}

		//f�� �ּҰ� ���ϱ�

		//������� �˻� �� �ε��� ����
		POINT min;
		int count2 = 0;
		for (UINT y = 0; y < HeightNode; y++)
		{
			for (UINT x = 0; x < WidthNode; x++)
			{
				//�˻� ��� ����
				if (Nodes.second.f &&
					Nodes.second.wall == false &&
					Nodes.second.close == false)
				{
					min.y = y;
					min.x = x;
					break;
				}
				count2++;
			}
		}

		//�� �̻� �ּҰ��� ã�� ���ϸ� �ݺ��� ����
		if(count2 == WidthNode * HeightNode)
		{ 
			break;
		}

		//���������� minF�� ���ϴ� ��!
		for (UINT y = 0; y < HeightNode; y++)
		{
			for (UINT x = 0; x < WidthNode; x++)
			{
				//�������� �ε����� �ٸ� ��庸�� F ���� ũ��
				if (nodes[min.y][min.x].second.f > Nodes.second.f &&
					Nodes.second.wall == false &&
					Nodes.second.close == false)
				{
					//���� ��带 �ε����� �ٽ� ����
					min.y = y;
					min.x = x;
				}
			}
		}

		//����
		if (min.x != 0)
		{
			if (nodes[min.y][min.x - 1].second.g > 10 + nodes[min.y][min.x].second.g)
			{
				nodes[min.y][min.x - 1].second.g = 10 + nodes[min.y][min.x].second.g;
				nodes[min.y][min.x - 1].second.parent.x = min.x;
				nodes[min.y][min.x - 1].second.parent.y = min.y;
			}
		}

		//������
		if (min.x != WidthNode - 1)
		{
			if (nodes[min.y][min.x + 1].second.g > 10 + nodes[min.y][min.x].second.g)
			{
				nodes[min.y][min.x + 1].second.g = 10 + nodes[min.y][min.x].second.g;
				nodes[min.y][min.x + 1].second.parent.x = min.x;
				nodes[min.y][min.x + 1].second.parent.y = min.y;
			}
		}

		//��
		if (min.y != 0)
		{
			if (nodes[min.y - 1][min.x].second.g > 10 + nodes[min.y][min.x].second.g)
			{
				nodes[min.y - 1][min.x].second.g = 10 + nodes[min.y][min.x].second.g;
				nodes[min.y - 1][min.x].second.parent.x = min.x;
				nodes[min.y - 1][min.x].second.parent.y = min.y;
			}
		}

		//�Ʒ�
		if (min.y != HeightNode - 1)
		{
			if (nodes[min.y + 1][min.x].second.g > 10 + nodes[min.y][min.x].second.g)
			{				
				nodes[min.y + 1][min.x].second.g = 10 + nodes[min.y][min.x].second.g;
				nodes[min.y + 1][min.x].second.parent.x = min.x;
				nodes[min.y + 1][min.x].second.parent.y = min.y;
			}
		}

		//�»�
		if (min.x != 0 && min.y != 0)
		{
			if (nodes[min.y - 1][min.x].second.wall == true &&
				nodes[min.y][min.x - 1].second.wall == true)
			{
				//����� ���� �ƴ� �� �׽�Ʈ
			}

			else if (nodes[min.y - 1][min.x - 1].second.g > 14 + nodes[min.y][min.x].second.g)
			{
				nodes[min.y - 1][min.x - 1].second.g = 14 + nodes[min.y][min.x].second.g;
				nodes[min.y - 1][min.x - 1].second.parent.x = min.x;
				nodes[min.y - 1][min.x - 1].second.parent.y = min.y;
			}
		}

		//����
		if (min.x != 0 && min.y != HeightNode - 1)
		{
			if (nodes[min.y + 1][min.x].second.wall == true &&
				nodes[min.y][min.x - 1].second.wall == true)
			{
				//����� ���� �ƴ� �� �׽�Ʈ
			}

			else if (nodes[min.y + 1][min.x - 1].second.g > 14 + nodes[min.y][min.x].second.g)
			{
				nodes[min.y + 1][min.x - 1].second.g = 14 + nodes[min.y][min.x].second.g;
				nodes[min.y + 1][min.x - 1].second.parent.x = min.x;
				nodes[min.y + 1][min.x - 1].second.parent.y = min.y;
			}
		}

		//���
		if (min.x != WidthNode - 1 && min.y != 0)
		{
			if (nodes[min.y - 1][min.x].second.wall == true &&
				nodes[min.y][min.x + 1].second.wall == true)
			{
				//����� ���� �ƴ� �� �׽�Ʈ
			}

			else if (nodes[min.y - 1][min.x + 1].second.g > 14 + nodes[min.y][min.x].second.g)
			{
				nodes[min.y - 1][min.x + 1].second.g = 14 + nodes[min.y][min.x].second.g;
				nodes[min.y - 1][min.x + 1].second.parent.x = min.x;
				nodes[min.y - 1][min.x + 1].second.parent.y = min.y;
			}
		}

		//����
		if (min.x != WidthNode - 1 && min.y != HeightNode - 1)
		{
			if (nodes[min.y + 1][min.x].second.wall == true &&
				nodes[min.y][min.x + 1].second.wall == true)
			{
				//����� ���� �ƴ� �� �׽�Ʈ
			}

			else if (nodes[min.y + 1][min.x + 1].second.g > 14 + nodes[min.y][min.x].second.g)
			{
				nodes[min.y + 1][min.x + 1].second.g = 14 + nodes[min.y][min.x].second.g;
				nodes[min.y + 1][min.x + 1].second.parent.x = min.x;
				nodes[min.y + 1][min.x + 1].second.parent.y = min.y;
			}
		}

		nodes[min.y][min.x].second.close = true;
				
		//�� �����
		if (nodes[min.y][min.x].second.goal == true)
		{
			POINT tempPoint; //minF �� ���� �ε��� ����
			tempPoint.y = min.y;
			tempPoint.x = min.x;

			POINT tempPoint2; //minF �� �θ� �ε��� ����

			while (true)
			{
				//�θ� ����
				tempPoint2.y = nodes[tempPoint.y][tempPoint.x].second.parent.y;
				tempPoint2.x = nodes[tempPoint.y][tempPoint.x].second.parent.x;

				//minF -> �θ�� ����
				tempPoint.y = tempPoint2.y;
				tempPoint.x = tempPoint2.x;

				way.push_back(tempPoint);

				//tempPoint�� ���� �ε����� �����ϸ� �ݺ��� ����
				if (nodes[tempPoint.y][tempPoint.x].second.start == true)
				{
					break;
				}
				//�θ� ���� �̻��� ���
				if (tempPoint.y == Inf)
				{
					Reset();
					break;
				}
				//���Ͱ� �Ѿ��� Ŀ�� ��
				if (way.size() == WidthNode * HeightNode)
				{
					Reset();
					break;
				}
			}
			break;
		}


	}//while
}



void PathFinder::Udpate(D3DXMATRIX& V, D3DXMATRIX& P)
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
	if (VK_ESCAPE) Reset();

	switch (nodeType)
	{
		case NodeType::Start: break;
		case NodeType::Goal: SetGoal(); SetStart(); break;
		case NodeType::Wall: SetWall(); break;
	}

	if (Key->Down(VK_RETURN))
		FindPath();
}

void PathFinder::Render()
{
	ImGui::Text("Node");
	ImGui::LabelText("startPos", "%0.2f, %0.2f", startPos.x, startPos.y);
	ImGui::LabelText("goalPos", "%0.2f, %0.2f", goalPos.x, goalPos.y);
	ImGui::LabelText("Node Type", "%d", nodeType);

	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{
			//������
			if (Nodes.second.start == true)
			{
				Nodes.first->Color(1, 1, 0); //���
			}

			//������
			else if (Nodes.second.goal == true)
			{
				Nodes.first->Color(0, 1, 0); //�ʷ�
			}

			//��
			else if (Nodes.second.wall == true)
			{
				Nodes.first->Color(1, 0, 0); //����
			}

			//����
			else if (Nodes.second.close == true)
			{
				Nodes.first->Color(0, 0, 1); //�Ķ�
			}

		}//for x
	}//for y

	//��
	for (UINT i = 0; i < way.size(); i++)
	{
		nodes[way[i].y][way[i].x].first->Color(1, 1, 1);
	}

	RenderText();

	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{
			Nodes.first->Render();
		}
	}

}

UINT PathFinder::FindH(int startX, int startY, int goalX, int goalY)
{
	int distanceX;
	int distanceY;
	int distance;

	distanceX = abs(startX - goalX);
	distanceY = abs(startY - goalY);

	return (distanceX + distanceY) * 10;
}

void PathFinder::Reset()
{
	for (UINT y = 0; y < HeightNode; y++)
	{
		for (UINT x = 0; x < WidthNode; x++)
		{			
			Nodes.second.start = false;
			Nodes.second.goal = false;
			Nodes.second.wall = false;
			Nodes.second.f = Inf;
			Nodes.second.g = Inf;
			Nodes.second.h = Inf;
			Nodes.second.close = false;
			Nodes.second.parent.x = Inf;
			Nodes.second.parent.y = Inf;

			Nodes.first->Color(0, 0, 0);
		}
	}

	way.clear();
	nodeType = NodeType::None;
}

D3DXVECTOR2 PathFinder::Position()
{
	return D3DXVECTOR2();
}

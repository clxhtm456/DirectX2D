#include "stdafx.h"
#include "NodeManager.h"
#include "Object/Node.h"

void NodeManager::PushBack_Node(Node * node)
{
	if (node == nullptr)
		return;

	nodeList.push_back(node);
}

void NodeManager::Erase_Node(Node * node)
{
	if (node == nullptr)
		return;

	for (auto iter = nodeList.begin(); iter != nodeList.end(); iter++) {
		if ((*iter) == node)
		{
			nodeList.erase(iter);
			return;
		}
	}
}

void NodeManager::Update(D3DXMATRIX & V, D3DXMATRIX & P)
{
	for (auto node : nodeList)
	{
		node->Update(V, P);
	}
}

void NodeManager::Render()
{
	for (auto node : nodeList)
	{
		node->Render();
	}
}

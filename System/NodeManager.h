#ifndef __NODE_MANAGER_H__
#define __NODE_MANAGER_H__

class NodeManager
{
private:
	vector<Node*> nodeList;
public:
	void PushBack_Node(Node* node);
	void Erase_Node(Node* node);
	void Update(D3DXMATRIX& V, D3DXMATRIX& P);
	void Render();
};

#endif
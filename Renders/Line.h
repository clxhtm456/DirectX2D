#ifndef __LINE_H__
#define __LINE_H__

class Line
{
public:
	Line(D3DXVECTOR2 leftPoint, D3DXVECTOR2 rightPoint);
	~Line();

	void Render();

public:
	void LeftPoint(D3DXVECTOR2& val){leftPoint = val;}
	D3DXVECTOR2 LeftPoint(){return leftPoint;}

	void RightPoint(D3DXVECTOR2& val){rightPoint = val;}
	D3DXVECTOR2 RightPoint(){return rightPoint;}

	float Slope() { return slope; }
	void DrawCollision(bool val) { bDrawCollision = val; }

private:
	struct Vertex
	{
		D3DXVECTOR3 Position;
	};
	Vertex vertices[2];

private:
	void CreateBuffer(wstring shader, D3DXVECTOR2 leftPoint, D3DXVECTOR2 rightPoint);

private:
	Shader * shader;
	ID3D11Buffer* vertexBuffer;

	D3DXVECTOR2 position;
	D3DXVECTOR2 scale;
	D3DXVECTOR2 leftPoint;//���ʳ� ��ǥ
	D3DXVECTOR2 rightPoint;//�����ʳ� ��ǥ

	float degree;//����
	float slope;//����

	bool bDrawCollision;//üũ�ڽ�
};

#endif
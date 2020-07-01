#include "stdafx.h"
#include "Line.h"

Line::Line(D3DXVECTOR2 leftPoint, D3DXVECTOR2 rightPoint):
	bDrawCollision(false),
	leftPoint(leftPoint),
	rightPoint(rightPoint),
	position(0,0),
	scale(1,1)
{
	wstring shader = Shaders + L"Bounding.fx";

	CreateBuffer(shader, leftPoint, rightPoint);

	if (leftPoint.x > rightPoint.x)
	{
		this->leftPoint = rightPoint;
		this->rightPoint = leftPoint;
	}

	slope = fabs(rightPoint.y - leftPoint.y) / (rightPoint.x - leftPoint.x);
}

Line::~Line()
{
	delete shader;
	SAFE_RELEASE( vertexBuffer);
}

void Line::Render()
{
	if (Director::getInstance()->DebugOption() == false)
		return;

	D3DXMATRIX V = *Director::getInstance()->MainCamera()->GetView();
	D3DXMATRIX P = *Director::getInstance()->MainCamera()->GetProjection();

	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

	D3DXMATRIX W, S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1.0f);
	D3DXMatrixTranslation(&T, position.x, position.y, 0.0f);

	W = S * T;
	shader->AsMatrix("World")->SetMatrix(W);

	vertices[0].Position = D3DXVECTOR3(leftPoint.x, leftPoint.y, 0);
	vertices[1].Position = D3DXVECTOR3(rightPoint.x, rightPoint.y, 0);

	DeviceContext->UpdateSubresource
	(
		vertexBuffer,
		0,
		NULL,
		vertices,
		sizeof(Vertex) * 2,
		0
	);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	shader->Draw(0, bDrawCollision ? 1 : 0, 2);
}

void Line::CreateBuffer(wstring shader, D3DXVECTOR2 leftPoint, D3DXVECTOR2 rightPoint)
{
	Line::shader = new Shader(shader);

	vertices[0].Position = D3DXVECTOR3(leftPoint.x, leftPoint.y,0.0f);
	vertices[1].Position = D3DXVECTOR3(rightPoint.x, rightPoint.y,0.0f);

	//CreateBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 2;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
}

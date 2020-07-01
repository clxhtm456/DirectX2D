#include "stdafx.h"
#include "Rect.h"

Rect::Rect(wstring shaderFile)
	:position(0, 0), scale(1, 1), color(1, 0, 0, 1)
{
	CreateBuffer(shaderFile);
	Color(color);
}

Rect::Rect(wstring shaderFile, D3DXVECTOR2 position, D3DXVECTOR2 scale, D3DXCOLOR color)
	:position(position), scale(scale), color(color)
{
	CreateBuffer(shaderFile);

	Color(color);
}

Rect::~Rect()
{
	SAFE_DELETE(shader);
	SAFE_RELEASE(vertexBuffer);
}

void Rect::Update()
{
	
}

void Rect::Render()
{
	D3DXMATRIX V = *Director::getInstance()->MainCamera()->GetView();
	D3DXMATRIX P = *Director::getInstance()->MainCamera()->GetProjection();

	D3DXMATRIX W, S, T;
	D3DXMatrixScaling(&S, scale.x, scale.y, 1);
	D3DXMatrixTranslation(&T, position.x, position.y, 0);
	W = S * T;

	shader->AsMatrix("World")->SetMatrix(W);
	shader->AsMatrix("View")->SetMatrix(V);
	shader->AsMatrix("Projection")->SetMatrix(P);

	shader->AsVector("Color")->SetFloatVector(color);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Draw(0, 0, 6);
}

void Rect::Position(float x, float y)
{
	D3DXVECTOR2 input = { x, y };
	Position(input);
}

void Rect::Position(D3DXVECTOR2 & vec)
{
	position = vec;
}

void Rect::Scale(float x, float y)
{
	D3DXVECTOR2 input = { x, y };
	Scale(input);
}

void Rect::Scale(D3DXVECTOR2 & vec)
{
	scale = vec;
}

void Rect::Color(float r, float g, float b)
{
	D3DXCOLOR input = { r, g, b,1};
	Color(input);
}

void Rect::Color(D3DXCOLOR & vec)
{
	color = vec;

	shader->AsVector("Color")->SetFloatVector(color);
}

bool Rect::PtInRect(Rect * rect, D3DXVECTOR2 position)
{
	float xScale = rect->scale.x*0.5f;
	float yScale = rect->scale.y *0.5f;
	
	float left = rect->position.x - xScale;
	float right = rect->position.x + xScale;
	float bottom = rect->position.y - yScale;
	float top = rect->position.y + yScale;

	bool bCheck = true;
	bCheck &= position.x > left;
	bCheck &= position.x < right;
	bCheck &= position.y > bottom;
	bCheck &= position.y < top;

	return bCheck;
}


void Rect::CreateBuffer(wstring shaderFile)
{
	shader = new Shader(shaderFile);


	//정점 위치 세팅
	Vertex vertices[6];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.0f);
	vertices[1].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[2].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[3].Position = D3DXVECTOR3(+0.5f, -0.5f, 0.0f);
	vertices[4].Position = D3DXVECTOR3(-0.5f, +0.5f, 0.0f);
	vertices[5].Position = D3DXVECTOR3(+0.5f, +0.5f, 0.0f);


	//Create VertexBuffer
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(Vertex) * 6;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = vertices;

		HRESULT hr = Device->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}
}

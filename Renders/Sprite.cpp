#include "stdafx.h"
#include "Sprite.h"

//-----------------------------------------------------------------------------------
//Sprite
//-----------------------------------------------------------------------------------

bool Sprite::Init(wstring spriteFile, float startX, float startY, float endX, float endY)
{
	if (!Node::Init())
		return false;
	bDrawBound = false;
	bDrawCollision = false;
	bDrawBloom = false;
	textureFile = spriteFile;

	shader = Director::getInstance()->DefaultShader();
	srv = Sprites::Load(spriteFile);
	
	//defaultWorld
	Position(0, 0);
	Scale(1, 1);
	Rotation(0, 0, 0);
	AnchorPosition(0.5f, 0.5f);
	D3DXMatrixIdentity(&world);
	depth = 1;

	sMap = shader->AsShaderResource("Map");
	sView = shader->AsMatrix("View");
	sProjection = shader->AsMatrix("Projection");
	sWorld = shader->AsMatrix("World");

	

	HRESULT hr;
	D3DX11_IMAGE_INFO info;
	hr = D3DX11GetImageInfoFromFile(spriteFile.c_str(), NULL, &info, NULL);
	assert(SUCCEEDED(hr));

	startX = (startX > 0) ? startX / (float)info.Width : 0.0f;
	startY = (startY > 0) ? startY / (float)info.Height : 0.0f;

	//end와 start가 겹침을 방지하기위해 end는 최소 1을 유지
	endX = (endX > 0) ? endX / (float)info.Width : 1.0f;
	endY = (endY > 0) ? endY / (float)info.Height : 1.0f;

	Vertex vertices[6];
	vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0);
	vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0);
	vertices[2].Position = D3DXVECTOR3(0.5f, -0.5f, 0);
	vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0);
	vertices[4].Position = D3DXVECTOR3(-0.5f, 0.5f, 0);
	vertices[5].Position = D3DXVECTOR3(0.5f, 0.5f, 0);

	//UV 포지션
	vertices[0].Uv = D3DXVECTOR2(startX,	endY);
	vertices[1].Uv = D3DXVECTOR2(startX,	startY);
	vertices[2].Uv = D3DXVECTOR2(endX,		endY);
	vertices[3].Uv = D3DXVECTOR2(endX,		endY);
	vertices[4].Uv = D3DXVECTOR2(startX,	startY);
	vertices[5].Uv = D3DXVECTOR2(endX,		startY);

	float sizeX = (endX > 0) ? endX * (float)info.Width : (float)info.Width;
	sizeX -= startX * (float)info.Width;

	float sizeY = (endY > 0) ? endY * (float)info.Height : (float)info.Height;
	sizeY -= startY * (float)info.Height;

	textureSize = D3DXVECTOR2(sizeX, sizeY);

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

	return true;
}


Sprite * Sprite::Create(wstring textureFile)
{
	Sprite* pRet = new Sprite();
	if(pRet && pRet->Init(textureFile,0,0,0,0))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

Sprite * Sprite::Create(wstring textureFile, float endX, float endY)
{
	Sprite* pRet = new Sprite();
	if (pRet && pRet->Init(textureFile, 0, 0, endX, endY))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

Sprite * Sprite::Create(wstring textureFile, float startX, float startY, float endX, float endY)
{
	Sprite* pRet = new Sprite();
	if (pRet && pRet->Init(textureFile, startX, startY, endX, endY))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = nullptr;
	}
	return pRet;
}

Sprite::~Sprite()
{
	SAFE_RELEASE(vertexBuffer);

	Sprites::Remove(textureFile);
}

void Sprite::Update()
{
	Node::Update();
}

void Sprite::Draw()
{
	if (_visible == false)
		return;

	D3DXMATRIX V = *Director::getInstance()->MainCamera()->GetView();
	D3DXMATRIX P = *Director::getInstance()->MainCamera()->GetProjection();

	sView->SetMatrix(V);
	sProjection->SetMatrix(P);

	D3DXMATRIX S, T, R;

	D3DXMatrixScaling(&S, _normalizedScale.x * textureSize.x, _normalizedScale.y * textureSize.y, 1.0f);//z값은 1은 넣어주기
	D3DXMatrixRotationYawPitchRoll(&R, _normalizedRotation.y, _normalizedRotation.x, _normalizedRotation.z);
	auto tempAnchor = _anchorPoint;
	{
		tempAnchor.x = abs(_anchorPoint.x - (_normalizedRotation.y / Math::PI));
		tempAnchor.y = abs(_anchorPoint.y - (_normalizedRotation.x / Math::PI));
	}
	//auto tempDepth = pow(2.72, -depth);
	D3DXMatrixTranslation(&T, (_normalizedPosition.x + _normalizedScale.x * textureSize.x * (0.5f-tempAnchor.x)),
		(_normalizedPosition.y + _normalizedScale.y * textureSize.y * (0.5f-tempAnchor.y)),
		0.0f);

	world = S * R * T;//scale * translation 순서유의

	sMap->SetResource(srv);
	sWorld->SetMatrix(world);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//shader->Draw(0, 0, 6);
	int shaderType = 0;
	if (bDrawBloom)
		shaderType = 1;
	else if (bDrawTwinkle)
		shaderType = 2;
	shader->Draw(0, shaderType, 6);

	

	/*if (bDrawBloom)
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		DeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader->Draw(0, (bDrawBloom == true) ? 1 : 0, 6);
	}*/
}

void Sprite::SetShader(wstring shaderFile)
{
}

void Sprite::DrawBloom(bool val, float r, float g, float b)
{
	DrawBloom(val, D3DXCOLOR(r, g, b,0));
}

void Sprite::DrawBloom(bool val, D3DXCOLOR color)
{
	bDrawBloom = val;
	shader->AsVector("Color")->SetFloatVector(color);
}

bool Sprite::Aabb(D3DXVECTOR2 targetPosition)
{
	return Aabb(this, targetPosition);
}

bool Sprite::Aabb(Sprite * b)
{
	return Aabb(this,b);
}

bool Sprite::Aabb(Sprite * sprite, D3DXVECTOR2 position)//영역과 점과의 충돌
{
	auto xScale = sprite->Scale().x * sprite->TextureSize().x * 0.5f;
	auto yScale = sprite->Scale().y * sprite->TextureSize().y * 0.5f;

	float left = sprite->Position().x - xScale;
	float right = sprite->Position().x + xScale;
	float bottom = sprite->Position().y - yScale;
	float top = sprite->Position().y + yScale;

	bool bCheck = true;
	bCheck &= position.x >= left;
	bCheck &= position.x <= right;
	bCheck &= position.y >= bottom;
	bCheck &= position.y <= top;

	return bCheck;
}

bool Sprite::Aabb(Sprite * a, Sprite * b)//영역과 영역충돌
{
	auto xScale = a->Scale().x * a->TextureSize().x * 0.5f;
	auto yScale = a->Scale().y * a->TextureSize().y * 0.5f;

	float leftA = a->Position().x - xScale;
	float rightA = a->Position().x + xScale;
	float bottomA = a->Position().y - yScale;
	float topA = a->Position().y + yScale;

	xScale = b->Scale().x * b->TextureSize().x * 0.5f;
	yScale = b->Scale().y * b->TextureSize().y * 0.5f;

	float leftB = b->Position().x - xScale;
	float rightB = b->Position().x + xScale;
	float bottomB = b->Position().y - yScale;
	float topB = b->Position().y + yScale;

	bool bCheck = true;
	bCheck &= leftA < rightB;
	bCheck &= rightA > leftB;

	bCheck &= topA > bottomB;
	bCheck &= bottomA < topB;

	return bCheck;
}

bool Sprite::Obb(Sprite * b)
{
	return Obb(this,b);
}

bool Sprite::Obb(Sprite * a, Sprite * b)
{
	ObbDesc obbA, obbB;

	D3DXVECTOR2 lengthA = D3DXVECTOR2(a->world._11, a->world._22)*0.5f;
	CreateObb(obbA, a->Position(), a->world, lengthA);

	D3DXVECTOR2 lengthB = D3DXVECTOR2(b->world._11, b->world._22)*0.5f;
	CreateObb(obbB, b->Position(), b->world, lengthB);
	return CheckObb(obbA,obbB);
}

void Sprite::CreateObb(OUT ObbDesc & out, D3DXVECTOR2 & position, D3DXMATRIX & world, D3DXVECTOR2 & length)
{
	out.Position = position;
	out.Length[0] = length.x;
	out.Length[1] = length.y;

	out.Direction[0] = D3DXVECTOR2(world._11, world._12);//x축 방향
	out.Direction[1] = D3DXVECTOR2(world._21, world._22);//y축 방향
														 
	//방향으로 처리하기위한 노멀라이즈
	D3DXVec2Normalize(&out.Direction[0], &out.Direction[0]);
	D3DXVec2Normalize(&out.Direction[1], &out.Direction[1]);
}

float Sprite::SeparateAxis(D3DXVECTOR2 & separate, D3DXVECTOR2 & e1, D3DXVECTOR2 & e2)
{
	float r1 = fabsf(D3DXVec2Dot(&separate, &e1));
	float r2 = fabsf(D3DXVec2Dot(&separate, &e2));

	return r1 + r2;
}

float Sprite::IA[4];
float Sprite::IB[4];
float Sprite::I[4];
float Sprite::ICheck[4];

bool Sprite::CheckObb(ObbDesc & obbA, ObbDesc & obbB)
{
	D3DXVECTOR2 nea1 = obbA.Direction[0];//normalized ea1
	D3DXVECTOR2 ea1 = nea1 * obbA.Length[0];//길이를 구함 scale을 쓰지않은 이유는 회전을 했을경우를 감안

	D3DXVECTOR2 nea2 = obbA.Direction[1];
	D3DXVECTOR2 ea2 = nea2 * obbA.Length[1];

	D3DXVECTOR2 neb1 = obbB.Direction[0];
	D3DXVECTOR2 eb1 = neb1 * obbB.Length[0];

	D3DXVECTOR2 neb2 = obbB.Direction[1];
	D3DXVECTOR2 eb2 = neb2 * obbB.Length[1];

	D3DXVECTOR2 direction = obbA.Position - obbB.Position;

	float lengthA	= 0.0f;
	float lengthB	= 0.0f;
	float length	= 0.0f;//totalLength

	

	//obbA 수평방향
	lengthA = D3DXVec2Length(&ea1);
	lengthB = SeparateAxis(nea1, eb1, eb2);//eb1, eb2의 내적:: a를 쳐다보는 각도의 길이를 구함
	length = fabsf(D3DXVec2Dot(&direction, &nea1));

	IA[0] = lengthA;
	IB[0] = lengthB;
	I[0] = length;
	ICheck[0] = length > lengthA + lengthB;

	if (length > lengthA + lengthB)
		return false;

	//obbA 수직방향
	lengthA = D3DXVec2Length(&ea2);
	lengthB = SeparateAxis(nea2, eb1, eb2);
	length = fabsf(D3DXVec2Dot(&direction, &nea2));

	IA[1] = lengthA;
	IB[1] = lengthB;
	I[1] = length;
	ICheck[1] = length > lengthA + lengthB;

	if (length > lengthA + lengthB)
		return false;

	//obbB 수평방향
	lengthA = D3DXVec2Length(&eb1);
	lengthB = SeparateAxis(neb1, ea1, ea2);
	length = fabsf(D3DXVec2Dot(&direction, &neb1));

	IA[2] = lengthA;
	IB[2] = lengthB;
	I[2] = length;
	ICheck[2] = length > lengthA + lengthB;

	if (length > lengthA + lengthB)
		return false;

	//obbB 수직방향
	lengthA = D3DXVec2Length(&eb2);
	lengthB = SeparateAxis(neb2, ea1, ea2);
	length = fabsf(D3DXVec2Dot(&direction, &neb2));

	IA[3] = lengthA;
	IB[3] = lengthB;
	I[3] = length;
	ICheck[3] = length > lengthA + lengthB;

	if (length > lengthA + lengthB)
		return false;

	return true;

}



//-----------------------------------------------------------------------------------
//Sprites
//-----------------------------------------------------------------------------------

map<wstring, Sprites::SpriteDesc> Sprites::spriteMap;

ID3D11ShaderResourceView * Sprites::Load(wstring file)
{
	if (spriteMap.count(file) > 0)
	{
		spriteMap[file].RefCount++;
		return spriteMap[file].SRV;
	}

	HRESULT hr;
	ID3D11ShaderResourceView* srv;
	hr = D3DX11CreateShaderResourceViewFromFile(Device, file.c_str(), NULL, NULL, &srv, NULL);
	assert(SUCCEEDED(hr));

	SpriteDesc desc;
	desc.RefCount++;
	desc.SRV = srv;
	spriteMap[file] = desc;

	return desc.SRV;
}

void Sprites::Remove(wstring file)
{
	UINT count = spriteMap.count(file);
	assert(count > 0);

	spriteMap[file].RefCount--;

	if (spriteMap[file].RefCount < 1)
	{
		SAFE_RELEASE(spriteMap[file].SRV);
		spriteMap.erase(file);
	}
}
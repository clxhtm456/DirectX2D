#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "Object/Node.h"

class Sprite : public Node
{
private:
	Shader* shader;
	ID3D11Buffer* vertexBuffer;

	ID3DX11EffectMatrixVariable* sWorld, * sView, * sProjection;
	ID3DX11EffectVectorVariable* sColor;
	ID3DX11EffectShaderResourceVariable* sMap;
	
	D3DXMATRIX world;

	D3DXVECTOR2 textureSize;
	float depth;


	wstring textureFile;
	ID3D11ShaderResourceView* srv;

	bool bDrawBound;
	bool bDrawCollision;
	bool bDrawBloom;
	bool bDrawTwinkle;

	struct Vertex
	{
		D3DXVECTOR3 Position;
		D3DXVECTOR2 Uv;
	};

private:
	static float IA[4];
	static float IB[4];
	static float I[4];
	static float ICheck[4];

private:
	

	void CreateBound();

public:	
	static Sprite* Create(wstring textureFile);
	static Sprite* Create(wstring textureFile, float endX, float endY);
	static Sprite* Create(wstring textureFile, float startX, float startY, float endX, float endY);
	bool Init(wstring spriteFile, float startX, float startY, float endX, float endY);
	virtual ~Sprite();

	virtual void Update();
	virtual void Draw();

	void SetShader(wstring shaderFile);

	void DrawBound(bool val) { bDrawBound = val; }
	void DrawCollision(bool val) { bDrawCollision = val; }
	void DrawBloom(bool val, float r, float g,float b);
	void DrawBloom(bool val, D3DXCOLOR color);
	void DrawTwinkle(bool val) { bDrawTwinkle = val; }

	bool Aabb(D3DXVECTOR2 targetPosition);
	bool Aabb(Sprite* b);

	static bool Aabb(Sprite* sprite, D3DXVECTOR2 position);
	static bool Aabb(Sprite* a, Sprite* b);

	bool Obb(Sprite* b);

	static bool Obb(Sprite* a, Sprite* b);

private:
	struct ObbDesc
	{
		D3DXVECTOR2 Position;//위치
		D3DXVECTOR2 Direction[2];// 수평방향 수직방향

		float Length[2];//단위벡터의 곱
	};

	/*
	position : 충돌 체크 시작위치
	world : 회전, 방향
	length : 어느정도의 길이를 체크할지
	*/
	static void CreateObb(OUT ObbDesc& out,D3DXVECTOR2& position,D3DXMATRIX& world,D3DXVECTOR2& length);
	static float SeparateAxis(D3DXVECTOR2& separate, D3DXVECTOR2& e1, D3DXVECTOR2& e2);
	static bool CheckObb(ObbDesc& obbA, ObbDesc& ObbB);

//GET&SET///////////////
public:

	D3DXVECTOR2 TextureSize() { return D3DXVECTOR2(textureSize.x*_scale.x, textureSize.y*_scale.y); }
};

//////////////////////////
//Sprite_Manager
//////////////////////////

class Sprites
{
private:
	friend class Sprite;
	friend class Sprite_Cutter;

private:
	static ID3D11ShaderResourceView* Load(wstring file);
	static void Remove(wstring file);

private:
	struct SpriteDesc
	{
		UINT RefCount = 0;
		ID3D11ShaderResourceView* SRV = nullptr;
	};

	static map<wstring, SpriteDesc> spriteMap;
};

#endif
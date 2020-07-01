#ifndef __ANIMATION_H__
#define __ANIMATION_H__

class Animation : public Node
{
private:
	int currentClip;
	vector<Clip*> clips;
public:
	static Animation* Create();
	bool Init();
	Animation();
	~Animation();

	void AddClip(Clip* clip);
	void Play(int clipNumber);
	void Stop();

	D3DXVECTOR2 TextureSize();

	void Update();
	void Render();

	int GetCurrentClip(){return currentClip;}


	void DrawBound(bool val);
	Clip* GetClip();
	Sprite* GetSprite();

	void DrawBloom(bool val, float r, float g, float b);
	void DrawBloom(bool val,D3DXCOLOR color);
	void DrawTwinkle(bool val);
};

#endif
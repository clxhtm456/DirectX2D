#ifndef __CLIP_H__
#define __CLIP_H__

enum class PlayMode
{
	End,
	Loop,
	Reverse
};

struct Frame
{
	Frame(Sprite* sprite, float playTime);
	~Frame();

	Sprite* image;
	float time;
};

class Clip : public Node
{
public:
	static Clip* Create(PlayMode mode = PlayMode::End, float speed = 1.0f);
	bool Init(PlayMode mode, float speed);
	Clip();
	~Clip();

	void AddFrame(Sprite* sprite, float playTime);
	void AddFrame(const function<void()>& func);

	void SetStartCallBack(const function<void()>& func);
	void SetEndCallBack(const function<void()>& func);

	D3DXVECTOR2 TextureSize();

	//���, ����
	void Play();
	void Play(UINT startFrame);
	void Stop();
	bool IsPlaying() { return bPlay; }

	void Update();
	void Render();

	virtual void AnchorPosition(float x, float y);
	virtual void AnchorPosition(D3DXVECTOR2 vec);

	void DrawBound(bool val);
	void DrawBloom(bool val, float r, float g, float b);
	void DrawBloom(bool val, D3DXCOLOR color);
	void DrawTwinkle(bool val);

	Sprite* GetSprite();
	PlayMode GetMode()
	{
		return mode;
	}

	bool EndFrame() { return !bPlay; }
private:
	float length;//Ŭ�� ��ü ����
	float speed;//��� ó�� ����
	bool bPlay;//��� ����
	UINT currentFrame;//���� �÷��� ���� ������ ��ȣ
	float playTime;//Ÿ�̸�

	PlayMode mode;
	vector<Frame*> frames;
	vector<pair<int, function<void()>>> callbackList;

	function<void()> startCallback;
	function<void()> endCallback;
};

#endif
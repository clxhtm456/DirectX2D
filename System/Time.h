#pragma once

#include <ctime>

#define MAX_DELTATIME 0.016f

class Time
{
private:
	Time();
	~Time();

	static Time* instance;

	static bool isTimerStopped;//Ÿ�̸� ����
	static float timeElapsed;//���� ���������κ��� ����ð�

	INT64 tickerPerSecond; //�ʴ� ƽ ī��Ʈ
	INT64 currentTime; //����ð�
	INT64 lastTime;// �����ð�
	INT64 lastFPSUpdate; //������ FPS ������Ʈ �ð�
	INT64 fpsUpdateInterval; //FPS ������Ʈ ����

	UINT frameCount; // �����Ӽ�
	float runningTime;//���� �ð�
	float framePerSecond;//FPS
public:
	static Time * getInstance()
	{
		assert(instance != nullptr);

		return instance;
	}
	static void Create()
	{
		assert(instance == nullptr);
		srand(time(NULL));
		instance = new Time();
	}
	static void Delete()
	{
		{ if (instance) { delete (instance); (instance) = nullptr; }}
	}

	static bool Stopped() { return isTimerStopped; }
	static float deltaTime() { return isTimerStopped ? 0.0f : timeElapsed > MAX_DELTATIME ? MAX_DELTATIME : timeElapsed; }

	void Update();

	void Start();
	void Stop();

	const float FPS() { return framePerSecond; }
	//���ٽ�
	//auto FPS()->float{return framePerSecond;}
	const float Running() { return runningTime; }
};
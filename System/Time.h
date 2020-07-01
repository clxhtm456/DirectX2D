#pragma once

#include <ctime>

#define MAX_DELTATIME 0.016f

class Time
{
private:
	Time();
	~Time();

	static Time* instance;

	static bool isTimerStopped;//타이머 중지
	static float timeElapsed;//이전 프레임으로부터 경과시간

	INT64 tickerPerSecond; //초당 틱 카운트
	INT64 currentTime; //현재시간
	INT64 lastTime;// 이전시간
	INT64 lastFPSUpdate; //마지막 FPS 업데이트 시간
	INT64 fpsUpdateInterval; //FPS 업데이트 간격

	UINT frameCount; // 프레임수
	float runningTime;//진행 시간
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
	//람다식
	//auto FPS()->float{return framePerSecond;}
	const float Running() { return runningTime; }
};
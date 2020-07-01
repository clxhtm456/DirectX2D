#include "stdafx.h"
#include "Time.h"

Time* Time::instance = nullptr;
bool Time::isTimerStopped = true;
float Time::timeElapsed = 0.0f;

Time::Time():
	tickerPerSecond(0),
	currentTime(0),
	lastTime(0),
	lastFPSUpdate(0),
	fpsUpdateInterval(0),
	frameCount(0),
	runningTime(0),
	framePerSecond(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER*)&tickerPerSecond);//타이머 진동주기 반환
	fpsUpdateInterval = tickerPerSecond >> 1;//fps 업데이트 주기 (대략 1초)
}

Time::~Time()
{
}

void Time::Update()
{
	if (isTimerStopped)
		return;

	//1. 현재 시간을 가져와서 시간 간격 및 진행 시간을 계산한다.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);//현재시간
	timeElapsed = (float)(currentTime - lastTime)/ (float)tickerPerSecond;//현재 타이머 - 이전타이머 / 타이머 주기
	runningTime += timeElapsed;//진행시간에 deltatime+

	//2. FPS 업데이트
	frameCount++;//Update 마다 frameCount증가
	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)//fps 업데이트 주기가 되었을경우(대략1초)
	{
		float tempCurrentTime = (float)currentTime / (float)tickerPerSecond;
		float tempLastTime = (float)lastFPSUpdate / (float)tickerPerSecond;
		framePerSecond = (float)frameCount / (tempCurrentTime - tempLastTime);//frame 계산 (대략 이전 카운트사이에 update가 몇번 불러와졌는가)

		lastFPSUpdate = (INT64)currentTime;//이전 fpsupdate 타이머 갱신
		frameCount = 0;//frameCount 초기화
	}
	lastTime = currentTime;//이전타이머 갱신
}

void Time::Start()
{
	if (!isTimerStopped)//타이머가 돌고있는경우 종료
		assert(false);

	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);//lastTime에 현재 타이머 기록
	isTimerStopped = false;//타이머 시작
}

void Time::Stop()
{
	if (isTimerStopped)//타이머가 안돌고있을경우 종료
		assert(false);

	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);//stopTime 에 현재 타이머 기록
	runningTime += (float)(stopTime - lastTime) / (float)tickerPerSecond;//running타임에 마지막 deltatime 기록
	isTimerStopped = true;//타이머 종료
}

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
	QueryPerformanceFrequency((LARGE_INTEGER*)&tickerPerSecond);//Ÿ�̸� �����ֱ� ��ȯ
	fpsUpdateInterval = tickerPerSecond >> 1;//fps ������Ʈ �ֱ� (�뷫 1��)
}

Time::~Time()
{
}

void Time::Update()
{
	if (isTimerStopped)
		return;

	//1. ���� �ð��� �����ͼ� �ð� ���� �� ���� �ð��� ����Ѵ�.
	QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);//����ð�
	timeElapsed = (float)(currentTime - lastTime)/ (float)tickerPerSecond;//���� Ÿ�̸� - ����Ÿ�̸� / Ÿ�̸� �ֱ�
	runningTime += timeElapsed;//����ð��� deltatime+

	//2. FPS ������Ʈ
	frameCount++;//Update ���� frameCount����
	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)//fps ������Ʈ �ֱⰡ �Ǿ������(�뷫1��)
	{
		float tempCurrentTime = (float)currentTime / (float)tickerPerSecond;
		float tempLastTime = (float)lastFPSUpdate / (float)tickerPerSecond;
		framePerSecond = (float)frameCount / (tempCurrentTime - tempLastTime);//frame ��� (�뷫 ���� ī��Ʈ���̿� update�� ��� �ҷ������°�)

		lastFPSUpdate = (INT64)currentTime;//���� fpsupdate Ÿ�̸� ����
		frameCount = 0;//frameCount �ʱ�ȭ
	}
	lastTime = currentTime;//����Ÿ�̸� ����
}

void Time::Start()
{
	if (!isTimerStopped)//Ÿ�̸Ӱ� �����ִ°�� ����
		assert(false);

	QueryPerformanceCounter((LARGE_INTEGER*)&lastTime);//lastTime�� ���� Ÿ�̸� ���
	isTimerStopped = false;//Ÿ�̸� ����
}

void Time::Stop()
{
	if (isTimerStopped)//Ÿ�̸Ӱ� �ȵ���������� ����
		assert(false);

	INT64 stopTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&stopTime);//stopTime �� ���� Ÿ�̸� ���
	runningTime += (float)(stopTime - lastTime) / (float)tickerPerSecond;//runningŸ�ӿ� ������ deltatime ���
	isTimerStopped = true;//Ÿ�̸� ����
}

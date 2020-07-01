#include "stdafx.h"
#include "CMouse.h"

#define CASE1	0

CMouse::CMouse(HWND hwnd):
	hwnd(hwnd),
	position(0,0),
	wheelValue(0),
	wheelMoveValue(0),
	wheelPrevValue(0)
{
	ZeroMemory(buttonStatus, sizeof(BYTE) * 8);
	ZeroMemory(buttonPrevStatus, sizeof(BYTE) * 8);
	ZeroMemory(buttonMap, sizeof(Button) * 8);
	ZeroMemory(buttonCount, sizeof(int) * 8);

	doubleClickTime = GetDoubleClickTime();//제어판의 더블클릭 타임을 받아옴
	startDoubleClickTime[0] = GetTickCount();//현재시간으로 초기화

	for (int i = 1; i < 8; i++)
	{
		startDoubleClickTime[i] = startDoubleClickTime[0];
	}
}

CMouse::~CMouse()
{
}

void CMouse::WndProc(UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage == WM_MOUSEMOVE)
	{
		position.x = static_cast<float>(LOWORD(lParam));
		position.y = static_cast<float>(HIWORD(lParam));
	}

	if (iMessage == WM_MOUSEWHEEL)
	{
		auto temp = static_cast<short>(HIWORD(wParam));

		wheelPrevValue = wheelValue;
		wheelValue += static_cast<float>(temp);
	}
}

void CMouse::Update()
{
	//auto hovering = ImGui::ismouse
	memcpy(buttonPrevStatus, buttonStatus, sizeof(BYTE) * 8);

	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x08000 ? 1 : 0;//왼쪽버튼
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x08000 ? 1 : 0;//오른쪽버튼
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x08000 ? 1 : 0;//가운데버튼

	for (UINT i = 0; i < 8; i++)
	{
		BYTE prevStatus = buttonPrevStatus[i];
		BYTE currentStatus = buttonStatus[i];

		if (prevStatus == 0 && currentStatus == 1)
		{
			buttonMap[i] = Button::Down;
		}
		else if (prevStatus == 1 && currentStatus == 0)
		{
			buttonMap[i] = Button::Up;
		}
		else if (prevStatus == 1 && currentStatus == 1)
		{
			buttonMap[i] = Button::Press;
		}
		else
		{
			buttonMap[i] = Button::None;
		}
	}

#if (CASE1==1)
	//더블클릭
	UINT buttonStatus = GetTickCount();
	for (UINT i = 0; i < 8; i++)
	{
		if (buttonMap[i] == Button::Down)
		{
			if (buttonCount[i] == 1)
			{
				auto timer = buttonStatus - startDoubleClickTime[i];
				if (timer <= doubleClickTime)
				{
					buttonMap[i] = Button::DoubleClick;
					buttonCount[i] = 0;
					return;
				}
				buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1)
			{
				startDoubleClickTime[i] = buttonStatus;
			}
		}
	}
#else
	//더블클릭
	UINT buttonStatus = GetTickCount();
	for (UINT i = 0; i < 8; i++)
	{
		if (buttonMap[i] == Button::Down)
		{
			if (buttonCount[i] == 1)
			{
				if (buttonStatus - startDoubleClickTime[i] >= doubleClickTime)
					buttonCount[i] = 0;
			}

			buttonCount[i]++;

			if (buttonCount[i] == 1)
				startDoubleClickTime[i] = buttonStatus;

		}

		if (buttonMap[i] == Button::Up)
		{
			if (buttonCount[i] == 1)
			{
				if (buttonStatus - startDoubleClickTime[i] >= doubleClickTime)
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)
			{
				if (buttonStatus - startDoubleClickTime[i] <= doubleClickTime)
					buttonMap[i] = Button::DoubleClick;

				buttonCount[i] = 0;
			}
		}
	}
#endif
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(hwnd, &point);

	wheelPrevValue = wheelValue;
	wheelMoveValue = wheelValue - wheelPrevValue;

}

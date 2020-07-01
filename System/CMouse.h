#ifndef __C_MOUSE_H__
#define __C_MOUSE_H__

class CMouse
{
private:
	enum class Button
	{
		None,
		Down,
		Up,
		Press,
		DoubleClick
	};

	enum class ButtonValue
	{
		LeftClick,
		RightClick,
		WheelClick
	};

public:
	CMouse(HWND hwnd);
	~CMouse();

	void WndProc(UINT iMessage, WPARAM wParam, LPARAM lParam);
	void Update();
	void Position(D3DXVECTOR2 value)
	{
		position = value;
	}
	D3DXVECTOR2 Position() { return position; }

	bool Down(UINT button)
	{
		return buttonMap[button] == Button::Down;
	}

	bool Up(UINT button)
	{
		return buttonMap[button] == Button::Up;
	}

	bool Press(UINT button)
	{
		return buttonMap[button] == Button::Press;
	}

	bool DoubleClick(UINT button)
	{
		return buttonMap[button] == Button::DoubleClick;
	}

	float Wheel() { return wheelMoveValue; }

private:
	HWND hwnd;

	D3DXVECTOR2 position;

	BYTE buttonStatus[8];
	BYTE buttonPrevStatus[8];
	Button buttonMap[8];

	float wheelValue;
	float wheelPrevValue;
	float wheelMoveValue;
	int buttonCount[8];

	DWORD doubleClickTime;
	DWORD startDoubleClickTime[8];
};

#endif
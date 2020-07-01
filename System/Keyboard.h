#pragma once
#include <bitset>

#define KEYMAX 256

class Keyboard
{
private:
	bitset<KEYMAX> up;
	bitset<KEYMAX> down;

public:
	Keyboard();
	~Keyboard();

	bool Down(int key);
	bool Up(int key);
	bool Press(int key);
	bool Toggle(int key);
};
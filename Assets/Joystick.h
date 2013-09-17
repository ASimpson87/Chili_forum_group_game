#pragma once
#include <Windows.h>
class Joystick
{
public: 
	Joystick();
	~Joystick();
	float GetJoyX();
	float GetJoyY();
	float GetJoyZ();
	float GetJoyR();
	unsigned int GetJoyButton();
	bool CheckJoystick();
	
private:
	int centerX, centerY, centerZ, centerR;
	JOYCAPS JoyCaps;
	JOYINFOEX JoyInfo;
};
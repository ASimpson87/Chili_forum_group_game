#include "Joystick.h"
#include <Windows.h>

Joystick::Joystick()
{
	ZeroMemory( &JoyCaps, sizeof(JOYCAPS) );
	joyGetDevCaps( 0, &JoyCaps, sizeof( JoyCaps ) );
	
	unsigned int dwSize = sizeof(JoyInfo);
	ZeroMemory(&JoyInfo, sizeof(JOYINFO));
	JoyInfo.dwSize = dwSize;
	JoyInfo.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNR | JOY_RETURNZ | JOY_RETURNBUTTONS;

	// Calculates the center of the joysticks.
	centerX = JoyCaps.wXmax / 2;
	centerY = JoyCaps.wYmax / 2;
	centerZ = JoyCaps.wZmax / 2;
	centerR = JoyCaps.wRmax / 2;
}

Joystick::~Joystick()
{
}

bool Joystick::CheckJoystick()
{
	if(joyGetPosEx(0, &JoyInfo) == JOYERR_NOERROR)
	{
		return true;
	}
	else
	{
		return false;
	}
}
float Joystick::GetJoyX()
{
	// Returns a normalized value between -1.0f and 1.0f for X axis.
	
	joyGetPosEx(0, &JoyInfo);
	float joyX = JoyInfo.dwXpos;
	joyX = joyX - centerX;
	joyX = joyX / centerX;
	return joyX;
}

float Joystick::GetJoyY()
{
	// Returns a normalized value between -1.0f and 1.0f for Y axis.
	joyGetPosEx(0, &JoyInfo);
	float joyY = JoyInfo.dwYpos;
	joyY = joyY - centerY;
	joyY = joyY / centerY;
	return joyY;
}

float Joystick::GetJoyZ()
{
	// Returns a normalized value between -1.0f and 1.0f for Z axis.
	joyGetPosEx(0, &JoyInfo);
	float joyZ = JoyInfo.dwZpos;
	joyZ = joyZ - centerZ;
	joyZ = joyZ / centerZ;
	return joyZ;
}

float Joystick::GetJoyR()
{
	// Returns a normalized value between -1.0f and 1.0f for R axis.
	joyGetPosEx(0, &JoyInfo);
	float joyR = JoyInfo.dwRpos;
	joyR = joyR - centerR;
	joyR = joyR / centerR;
	return joyR;
}

unsigned int Joystick::GetJoyButton()
{
	// Returns which joystick button was pressed
	return JoyInfo.dwButtons;
}
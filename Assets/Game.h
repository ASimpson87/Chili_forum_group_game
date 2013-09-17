/****************************************************************************************** 
 *	Chili DirectX Framework Version 12.10.21											  *	
 *	Game.h																				  *
 *	Copyright 2012 PlanetChili.net														  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once

#include "D3DGraphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Sound.h"
#include "Timer.h"
#include "FrameTimer.h"
#include "Surface.h"
#include "Joystick.h"
#include "Ship.h"
#include "DInput.h"


class Game
{
public:
	Game( HWND hWnd,KeyboardServer& kServer,const MouseServer& mServer );
	~Game();
	void Go();

	// Game states: ShowTitle, PlayGame, Pause

private:
	enum GameState
	{
		Title, ScoreBoard, Play, Pause, Poll
	};

	struct Joynfo
	{
		float x,y;
	};

private:
	void ComposeFrame();
	/********************************/
	/*  User Functions              */
	void DoState( GameState gState );
	void Game::PolljStick();
	void DrawShip(Ship* ship);
	void PollKeyboard();
	void Clip();
	Joynfo RemoveDeadZone(float JoyX, float JoyY);
	void DrawAmmo();
	/********************************/
private:

	D3DGraphics gfx;
	KeyboardClient kbd;
	MouseClient mouse;
	DSound audio;
	/********************************/
	/*  User Variables              */
	GameState gState;
	JOYINFO JoyInfo;
	Joystick GamePad;
	bool hasJoystick;
	float JX;
	float JY;
	float jPosXLastFrame, jPosYLastFrame;
	Timer powerUpTimer;
	Player p1ship;
	Enemy1 e1ship;
	float deadZone;
	Timer fireRate;
	/********************************/
};
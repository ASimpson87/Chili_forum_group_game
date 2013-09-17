/****************************************************************************************** 
 *	Chili DirectX Framework Version 12.10.21											  *	
 *	Game.cpp																			  *
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
#include "Game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <float.h>
#include "Surface.h"

Game::Game( HWND hWnd,KeyboardServer& kServer,const MouseServer& mServer )
:	gfx( hWnd ),
	audio( hWnd ),
	kbd( kServer ),
	mouse( mServer ),
	gState(Play), 
	JX(400),
	JY(300),
	jPosXLastFrame(0),
	jPosYLastFrame(0)	
{
	srand( (unsigned int)time( NULL ) );	
	hasJoystick = GamePad.CheckJoystick();
}

Game::~Game()
{
}

void Game::DoState( Game::GameState gState )
{
	switch (gState)
	{
	case Title:
		{
			
		}
	case ScoreBoard:
		{
		}
	case Play:
		{
			if(hasJoystick)
			{
				PolljStick();
			}
			else
			{
				PollKeyboard();
			}
			Clip();
			p1ship.Move(JX, JY);
		}
	case Pause:
		{
		}	
	}
		
}

void Game::Go()
{
	gfx.BeginFrame();
	DoState(gState);	
	ComposeFrame();
	gfx.EndFrame();
}

void Game::PollKeyboard()
{
	unsigned int Speed = p1ship.GetSpeed();
	// W
	if( kbd.KeyIsPressed(0x57))
	{
		JY = JY - Speed;
	}
	// S
	if( kbd.KeyIsPressed( 0x53 ) )
	{
		JY = JY + Speed;
	}
	// A
	if( kbd.KeyIsPressed( 0x41 ) )
	{
		JX = JX - Speed;
	}
	// D
	if( kbd.KeyIsPressed( 0x44))
	{
		JX = JX + Speed;
	}
	if( kbd.KeyIsPressed( VK_SPACE ))
	{
		p1ship.Fire();
	}
}

void Game::Clip()
{
	int Radius = 16;
	if(JX <= Radius )
	{
		JX = Radius;
	}
	if(JY <= Radius )
	{
		JY = Radius;
	}
	if(JX > SCREENWIDTH - Radius)
	{
		JX = SCREENWIDTH - Radius;
	}
	if(JY > SCREENHEIGHT - Radius)
	{
		JY = SCREENHEIGHT - Radius;
	}
	
}

Game::Joynfo Game::RemoveDeadZone(float JoyX, float JoyY)
{	
	if(JoyX < 0.05f && JoyX > -0.05f)
	{
		JoyX = 0.0f;
	}
	if(JoyY < 0.05f && JoyY > -0.05f)
	{
		JoyY = 0.0f;
	}
	Joynfo jnfo = {JoyX, JoyY};
	return jnfo;
}

void Game::PolljStick()
{
	float temp = fireRate.GetTimeMilli();
	
	// Get X and Y position for joystick
	
	float JoyX = GamePad.GetJoyX();
	float JoyY = GamePad.GetJoyY();
	float JoyR = GamePad.GetJoyR();
	float JoyZ = GamePad.GetJoyZ();

	// Set deadzone for joystick X and Y to +- deadZone
	Joynfo jnfo = RemoveDeadZone(JoyX, JoyY);
	float Speed = p1ship.GetSpeed();
	JX = JX + (jnfo.x * Speed);
	JY = JY + (jnfo.y * Speed);
	if( GamePad.GetJoyButton() == 1)
	{
		if(temp > 100)
		{
			fireRate.StopWatch();
			p1ship.Fire();
			unsigned int alSize = p1ship.GetAmmoListSize();
			fireRate.StartWatch();
		}
	}
}

void Game::DrawShip(Ship* ship)
{
	Ship::Vertices* verts = ship->GetVertList();
	unsigned int ShipX = ship->GetShipX();
	unsigned int ShipY = ship->GetShipY();
	unsigned int nVerts = ship->GetNumVerts(); 
	unsigned int shieldRad = ship->GetShieldRad();

	for(int i = 0; i < nVerts; i++)
	{
		int j = (i+1) % nVerts;
					
		gfx.DrawShip(ShipX, ShipY, verts[i].vx, verts[i].vy,
			verts[j].vx, verts[j].vy, D3DCOLOR_XRGB(255, 255, 255));
	}
	gfx.DrawShield(ShipX, ShipY, shieldRad, 1.0f);

}

void Game::DrawAmmo()
{
	unsigned int alSize = p1ship.GetAmmoListSize();
	Ship::Vertices Ammo = {0, 0};
	p1ship.UpdateAmmo(alSize);
	alSize = p1ship.GetAmmoListSize();
	for(int i = 0; i < alSize; i++)
	{
		Ammo = p1ship.GetAmmoPos(i);
		if((Ammo.vx > 0 && Ammo.vx < SCREENWIDTH-5) && (Ammo.vy > 0 && Ammo.vy < SCREENHEIGHT-5))
		{
			gfx.DrawAmmo(Ammo.vx, Ammo.vy);
		}
		else
		{
			p1ship.KillAmmo(i);
			alSize = p1ship.GetAmmoListSize();
		}
	}
}

void Game::ComposeFrame()
{
	
	DrawShip(&p1ship);
	DrawShip(&e1ship);
	DrawAmmo();

}
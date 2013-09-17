#pragma once
#include "Projectiles.h"
#include <vector>
#include "Timer.h"

class Ship
{
public:
	Ship();
	~Ship();
	enum ShipState
	{
		Moving,
		Firing,
		Dead
	};
	
	// Structure to hold the X and Y positions of the vertices of each ship
	// Can use DrawLine in the D3DGraphics class to connect each vertex
	struct Vertices
	{
		float vx, vy;		
	};
	
	struct Shield
	{
		unsigned int strength, rechargeRate, respawnMax;
	};

	virtual void Move (const unsigned int x, const unsigned int y) = 0;
	virtual void Fire () = 0;
	virtual void Die () = 0;

	unsigned int GetNumVerts()const;
	float GetSpeed();
	unsigned int GetShipX();
	unsigned int GetShipY();
	Vertices* GetVertList();
	unsigned int GetShieldRad();
	void KillAmmo(unsigned int ammoIndex);
	void UpdateAmmo(unsigned int ammoListSize);
	unsigned int GetAmmoListSize();
	Vertices GetAmmoPos(unsigned int ammoIndex);

protected:
	unsigned int ShipX, ShipY;
	float Speed;
	Shield shield;
	Vertices* verts;
	unsigned int numVerts;
	unsigned int shieldRad;
	std::vector<Projectile> ammoVector;
	Timer shipTimer;
	float startFireTime, stopFireTime, startMoveTime, stopMoveTime;


};

class Player: public Ship
{
public:
	Player();
	~Player();
	void Move(const unsigned int ScrnX, const unsigned int ScrnY);
	void Fire();
	void Die();
};

class Enemy1: public Ship
{
public:
	Enemy1();
	~Enemy1();
	
	void Move(const unsigned int ScrnX, const unsigned int ScrnY);
	void Fire();
	void Die();


	
};

class Enemy2: public Ship
{
public:
	Enemy2();
	~Enemy2();
	void Move(const unsigned int ScrnX, const unsigned int ScrnY);
	void Fire();
	void Die();
};
#include "Ship.h"
/*  The Ship class is the parent class and will store all functions that are not
	specific to any one ship and it will store virtual functions that will be
	defined in child classes.  Any data that is ship specific will be listed in
	the child class while common data such as speed will be stored in the parent 
	class as protected with a Get function for retrieving the data.*/

/*  Ships can be created by listing the X and Y distance from the center of the ship
	in the constructor of the child class.
	If ships take up 32x32 then center would be x=15, y=15
	List the vertices either in a clockwise or anticlockwise manner, as long as each
	point connects, the DrawShip function in Game will connect the last point in the 
	list to the first point. */

Ship::Ship()
	:
	ShipX(0),
	ShipY(0),
	Speed(0)
{
}

float Ship::GetSpeed()
{
	stopMoveTime = shipTimer.GetTimeMilli();	
	float temp = stopMoveTime - startMoveTime;
	startMoveTime = shipTimer.GetTimeMilli();
	float shipSpeed = Speed / 1000 * temp;
	return shipSpeed;
}

unsigned int Ship::GetShieldRad()
{
	return shieldRad;
}

unsigned int Ship::GetNumVerts()const
{
	return numVerts;
}

unsigned int Ship::GetShipX()
{
	return ShipX;
}

unsigned int Ship::GetShipY()
{
	return ShipY;
}

Ship::Vertices* Ship::GetVertList()
{
	return verts;
}

void Ship::KillAmmo(unsigned int ammoIndex)
{
	ammoVector.erase(ammoVector.begin() + ammoIndex);
}

void Ship::UpdateAmmo(unsigned int ammoListSize)
{
	for(int i = 0; i < ammoListSize; i++)
	{
		if(ammoVector[i].HasTimeToLive())
		{			
			ammoVector[i].Move(true);
		}
		else
		{
			KillAmmo(i);
			if(ammoListSize != ammoVector.size())
			{
				ammoListSize = ammoVector.size();
			}
		}
	}
}

Ship::Vertices Ship::GetAmmoPos(unsigned int ammoIndex)
{
	Vertices Ammo = { ammoVector[ammoIndex].x, ammoVector[ammoIndex].y};
	return Ammo;
}

unsigned int Ship::GetAmmoListSize()
{
	return ammoVector.size();
}

Ship::~Ship()
{
}

//  ------------End of Ship class-------------


Player::Player()	
{
	shipTimer.StartWatch();
	startFireTime = startMoveTime = shipTimer.GetTimeMilli();
	Speed = 350;													// Pixels per second
	numVerts = 4;
	shieldRad = 40;
	verts = new Vertices[numVerts];
	// Point 1
	verts[0].vx = 0;
	verts[0].vy = -15;
	// Point 2
	verts[1].vx = -10;
	verts[1].vy = 15;
	// Point 3
	verts[2].vx = 0;
	verts[2].vy = 10;
	// Point 4
	verts[3].vx = 10;
	verts[3].vy = 15;
}

void Player::Move(unsigned int scrnX, unsigned int scrnY)
{	
	ShipX = scrnX;
	ShipY = scrnY;
}



void Player::Fire()
{	
	Projectile ammo(ShipX, ShipY);
	ammoVector.push_back(ammo);	
}

void Player::Die()
{
}



Player::~Player()
{
}

// *************************************** End of Hero Ship Class

Enemy1::Enemy1()
{
	ShipX = 32;
	ShipY = 32;
	Speed = 3;
	numVerts = 4;
	shieldRad = 0;
	verts = new Vertices[numVerts];
	// Point 1
	verts[0].vx = 0;
	verts[0].vy = -15;
	// Point 2
	verts[1].vx = -10;
	verts[1].vy = 0;
	// Point 3
	verts[2].vx = 0;
	verts[2].vy = 15;
	// Point 4
	verts[3].vx = 10;
	verts[3].vy = 0;
}

void Enemy1::Move(unsigned int scrnX, unsigned int scrnY)
{
	ShipX = scrnX;
	ShipY = scrnY;
}

void Enemy1::Fire()
{
}

void Enemy1::Die()
{
}


Enemy1::~Enemy1()
{
}

Enemy2::Enemy2()
{
	shieldRad = 0;
}

void Enemy2::Move(unsigned int scrnX, unsigned int scrnY)
{
	ShipX = scrnX;
	ShipY = scrnY;
}

void Enemy2::Fire()
{
}

void Enemy2::Die()
{
}

Enemy2::~Enemy2()
{
}
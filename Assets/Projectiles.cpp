#include "Projectiles.h"


Projectile::Projectile(unsigned int originX, unsigned int originY)
	:
	TTL(2 * 1000),	
	speed(800),
	x(originX),
	y(originY)
{
	timer.StartWatch();
	startMoveTime = startLiveTime = timer.GetTimeMilli();
}

void Projectile::Move(bool isFromPlayer)
{
	stopMoveTime = timer.GetTimeMilli();
	float temp = stopMoveTime - startMoveTime;
	float a = speed / 1000;
	if(isFromPlayer)
	{
		y -= a * temp;
	}
	else
	{
		y += a * temp;
	}
	startMoveTime = timer.GetTimeMilli();

}

bool Projectile::HasTimeToLive()
{
	stopLiveTime = timer.GetTimeMilli();
	if(stopLiveTime < TTL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Projectile::~Projectile(void)
{
	
}

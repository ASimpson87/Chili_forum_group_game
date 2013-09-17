#pragma once
#include "Timer.h"

class Projectile
{
public:
	Projectile(unsigned int originX, unsigned int originY);
	~Projectile(void);
	
	void Move( bool isFromPlayer);
	bool HasTimeToLive();

	Timer timer;
	float x, y, speed, TTL;
	float startLiveTime, stopLiveTime, startMoveTime, stopMoveTime;
};


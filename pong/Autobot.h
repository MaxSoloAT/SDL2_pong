#pragma once
#include "Paddle.h"
#include <queue>

class Autobot : public Paddle
{	
public:
	
	std::queue<int> bot_delay;

	Autobot(Vec2 position, Vec2 velocity)
		: Paddle (position, velocity)
	{
	}

	//Update for bot
	void AUpdate(float dt, int p);

	//Fill the q 
	void CreateDelay();
	
};


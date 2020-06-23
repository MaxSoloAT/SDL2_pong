#include "Autobot.h"
#include "Ball.h"

void Autobot::AUpdate(float dt, int p)
{
	position.y = p - PADDLE_HEIGHT / 2.0f;
	if (position.y < 0)
	{
		position.y = 0;
	}
	else if (position.y > (WINDOW_HEIGHT - PADDLE_HEIGHT))
	{
		position.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
	}
}

void Autobot::CreateDelay() {
	for (int i = 0; i < 300; i++)
	{
		bot_delay.push(WINDOW_HEIGHT / 2.0f);
	}
}
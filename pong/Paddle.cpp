#include "Paddle.h"

void Paddle::Update(float dt)
{
	position +=  velocity * dt ;
	if (position.y < 0)
	{
		position.y = 0;
	}
	else if (position.y > (WINDOW_HEIGHT - PADDLE_HEIGHT))
	{
		position.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
	}
}

void Paddle::Draw(SDL_Renderer* renderer)
{
	rect.y = static_cast<int>(position.y);
	SDL_RenderFillRect(renderer, &rect);
}

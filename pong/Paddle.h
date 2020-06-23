#pragma once
#include"Vec2.h"

class Paddle
{
public:
	Vec2 position;
	Vec2 velocity;
	SDL_Rect rect{};

	Paddle(Vec2 position, Vec2 velocity)
		: position(position), velocity(velocity)
	{
		rect.x = static_cast<int>(position.x);
		rect.y = static_cast<int>(position.y);
		rect.w = PADDLE_WIDTH;
		rect.h = PADDLE_HEIGHT;
	}

	//Update position
	void Update(float dt);

	//Draw paddle
	void Draw(SDL_Renderer* renderer);
	
	
};


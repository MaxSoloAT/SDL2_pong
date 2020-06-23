#pragma once
#include "Vec2.h"

class Ball
{
public:
	Vec2 position;
	Vec2 velocity;
	SDL_Rect rect;

	Ball(Vec2 position, Vec2 velocity)
		: position(position), velocity(velocity)
	{
		rect.x = static_cast<int>(position.x);
		rect.y = static_cast<int>(position.y);
		rect.w = BALL_WIDTH;
		rect.h = BALL_HEIGHT;
	}

	//Update position
	void Update(float dt);

	//Draw the ball
	void Draw(SDL_Renderer* renderer);

	//Check collide with paddle
	void CollideWithPaddle(Contact const& contact, float dt);

	//Check collide with wall
	void CollideWithWall(Contact const& contact);
};


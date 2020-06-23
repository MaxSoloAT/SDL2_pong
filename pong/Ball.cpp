#include "Ball.h"
#include <iostream>

void Ball::Update(float dt)
{
	position += velocity * dt;
}

void Ball::Draw(SDL_Renderer* renderer)
{
	rect.x = static_cast<int>(position.x);
	rect.y = static_cast<int>(position.y);
	SDL_RenderFillRect(renderer, &rect);
}

void Ball::CollideWithPaddle(Contact const& contact, float dt)
{
	//Mid collision
	position.x += contact.penetration;
	velocity.x = -velocity.x ;

	//New random
	int Idt = dt * 100;
	srand(Idt);
	float rnd = rand() % 40;

	if (contact.type == CollisionType::Top)
	{
		velocity.y = -1 * (rnd / 100 + .70f) * BALL_SPEED;
	}
	else if (contact.type == CollisionType::Bottom)
	{
		velocity.y = (rnd / 100 + .70f) * BALL_SPEED;
	}
}

void Ball::CollideWithWall(Contact const& contact)
{
	if ((contact.type == CollisionType::Top)
		|| (contact.type == CollisionType::Bottom))
	{
		position.y += contact.penetration;
		velocity.y = -velocity.y;
	}
	//Someone lose
	else if (contact.type == CollisionType::Left)
	{
		position.x = WINDOW_WIDTH / 2.0f;
		position.y = WINDOW_HEIGHT / 2.0f;
		velocity.x = BALL_SPEED;
		velocity.y = 0.0f;
	}
	else if (contact.type == CollisionType::Right)
	{
		position.x = WINDOW_WIDTH / 2.0f;
		position.y = WINDOW_HEIGHT / 2.0f;
		velocity.x = -BALL_SPEED;
		velocity.y = 0.0f;
	}
}
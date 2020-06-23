#include <chrono>
#include "Ball.h"
#include "Paddle.h"
#include "PlayerScore.h"
#include "Autobot.h"
#include <SDL_mixer.h>

//Checking where ball and paddle collision
Contact CheckPaddleCollision(Ball const& ball, Paddle const& paddle)
{
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + BALL_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + BALL_HEIGHT;

	float paddleLeft = paddle.position.x;
	float paddleRight = paddle.position.x + PADDLE_WIDTH;
	float paddleTop = paddle.position.y;
	float paddleBottom = paddle.position.y + PADDLE_HEIGHT;

	Contact contact{};
	
	if (ballLeft >= paddleRight)
	{
		return contact;
	}

	if (ballRight <= paddleLeft)
	{
		return contact;
	}

	if (ballTop >= paddleBottom)
	{
		return contact;
	}

	if (ballBottom <= paddleTop)
	{
		return contact;
	}
	
	float paddleRangeUpper = paddleBottom - (2.0f * PADDLE_HEIGHT / 3.0f);
	float paddleRangeMiddle = paddleBottom - (PADDLE_HEIGHT / 3.0f);

	if (ball.velocity.x < 0)
	{
		//My paddle
		contact.penetration = paddleRight - ballLeft;
	}
	else if (ball.velocity.x > 0)
	{
		//Autobot`s paddle
		contact.penetration = paddleLeft - ballRight;
	}

	//The place where ball and paddle collision  
	if ((ballBottom > paddleTop)
		&& (ballBottom < paddleRangeUpper))
	{
		contact.type = CollisionType::Top;
	}
	else if ((ballBottom > paddleRangeUpper)
		&& (ballBottom < paddleRangeMiddle))
	{
		contact.type = CollisionType::Middle;
	}
	else
	{
		contact.type = CollisionType::Bottom;
	}
	return contact;
}

//Checking where ball and wall collision
Contact CheckWallCollision(Ball const& ball)
{
	float ballLeft = ball.position.x;
	float ballRight = ball.position.x + BALL_WIDTH;
	float ballTop = ball.position.y;
	float ballBottom = ball.position.y + BALL_HEIGHT;

	Contact contact{};
	//The place where ball and wall collision  
	if (ballLeft < 0.0f)
	{
		contact.type = CollisionType::Left;
	}
	else if (ballRight > WINDOW_WIDTH)
	{
		contact.type = CollisionType::Right;
	}
	else if (ballTop < 0.0f)
	{
		contact.type = CollisionType::Top;
		contact.penetration = -ballTop;
	}
	else if (ballBottom > WINDOW_HEIGHT)
	{
		contact.type = CollisionType::Bottom;
		contact.penetration = WINDOW_HEIGHT - ballBottom;
	}

	return contact;
}

//Drawing the text with selected color and background
void  MenuLineDraw(SDL_Renderer* renderer, TTF_Font* scoreFont, std::string tp, int y, int x,SDL_Color colort ,SDL_Color colorback ) {
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Rect rect;
	surface = TTF_RenderText_Shaded(scoreFont, tp.c_str(), colort, colorback);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
	SDL_RenderPresent(renderer);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
}

int main(int argc, char* argv[])
{
	// Initializ SDL components
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	TTF_Init();
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	
	//Window
	SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	
	//Sound and font
	Mix_Chunk* HitSound = Mix_LoadWAV("ball.wav");
	TTF_Font* scoreFont = TTF_OpenFont("19517.otf", 100);
	TTF_Font* littelFont = TTF_OpenFont("19517.otf", 30);
	
	//Game menu drawing
	bool bot = false;
	bool running = true;
	{
		bool start = false;
		MenuLineDraw(renderer, scoreFont, "PING-PONG", 50, WINDOW_WIDTH / 2 - 150, { 255,255,255 }, { 0,0,0 });
		MenuLineDraw(renderer, scoreFont, "1 VS 1", 250, WINDOW_WIDTH / 2 - 100, {0,0,0}, { 255,255,255 });
		MenuLineDraw(renderer, scoreFont, "1 VS BOT", 320, WINDOW_WIDTH / 2 - 100, { 255,255,255 }, { 0,0,0 });
		MenuLineDraw(renderer, littelFont, "W/S for player 1", 605, 20, { 255,255,255 }, { 0,0,0 });
		MenuLineDraw(renderer, littelFont, "UP/DOWN for player 2", 645, 20, { 255,255,255 }, { 0,0,0 });
		MenuLineDraw(renderer, littelFont, "SPACE for start", 685, 20, { 255,255,255 }, { 0,0,0 });
		while (true)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				//Seletect type of game
				if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_SPACE)
					{
						start = true;
					}
					if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
					{
						MenuLineDraw(renderer, scoreFont, "PING-PONG", 50, WINDOW_WIDTH / 2 - 150, { 255,255,255 }, { 0,0,0 });
						MenuLineDraw(renderer, scoreFont, "1 VS 1", 250, WINDOW_WIDTH / 2 - 100, { 0,0,0 }, { 255,255,255 });
						MenuLineDraw(renderer, scoreFont, "1 VS BOT", 320, WINDOW_WIDTH / 2 - 100, { 255,255,255 }, { 0,0,0 });
						MenuLineDraw(renderer, littelFont, "W/S for player 1", 605, 20, { 255,255,255 }, { 0,0,0 });
						MenuLineDraw(renderer, littelFont, "UP/DOWN for player 2", 645, 20, { 255,255,255 }, { 0,0,0 });
						MenuLineDraw(renderer, littelFont, "SPACE for start", 685, 20, { 255,255,255 }, { 0,0,0 }); 
						bot = false;
					}
					if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
					{
						MenuLineDraw(renderer, scoreFont, "PING-PONG", 50, WINDOW_WIDTH / 2 - 150, { 255,255,255 }, { 0,0,0 });
						MenuLineDraw(renderer, scoreFont, "1 VS 1", 250, WINDOW_WIDTH / 2 - 100, { 255,255,255 }, { 0,0,0 });
						MenuLineDraw(renderer, scoreFont, "1 VS BOT", 320, WINDOW_WIDTH / 2 - 100, { 0,0,0 }, { 255,255,255 });
						MenuLineDraw(renderer, littelFont, "W/S for player 1", 605, 20, { 255,255,255 }, { 0,0,0 });
						MenuLineDraw(renderer, littelFont, "UP/DOWN for player 2", 645, 20, { 255,255,255 }, { 0,0,0 });
						MenuLineDraw(renderer, littelFont, "SPACE for start", 685, 20, { 255,255,255 }, { 0,0,0 }); 
						bot = true;
					}
				}
				SDL_RenderClear(renderer);
			}
			if (start)
			{
				break;
			}
		}
	}

	TTF_CloseFont(littelFont);

	//Game process	
	{
		// Create the player score
		PlayerScore playerOneScoreText(Vec2(WINDOW_WIDTH / 4, 20), renderer, scoreFont);
		PlayerScore playerTwoScoreText(Vec2(3 * WINDOW_WIDTH / 4, 20), renderer, scoreFont);

		//Create the Ball
		Ball ball(Vec2(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f),	Vec2(BALL_SPEED, 0.0f));

		//Create the paddles/bot
		Paddle paddleOne(Vec2(50.0f, WINDOW_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f), Vec2(0.0f, 0.0f));
		Autobot paddleTwo(Vec2(WINDOW_WIDTH - 50.0f, WINDOW_HEIGHT / 2.0f - PADDLE_HEIGHT / 2.0f),	Vec2(0.0f, 0.0f));
		
		if (bot) {
			paddleTwo.CreateDelay();
		}

		int playerOneScore = 0;
		int playerTwoScore = 0;
		bool buttons[4] = {};
		float dt = 0.0f;

		while (running)
		{
			auto startTime = std::chrono::high_resolution_clock::now();

			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				//The key processing
				if (event.type == SDL_QUIT)
				{
					running = false;
				}
				else if (event.type == SDL_KEYDOWN)
				{
					if (event.key.keysym.sym == SDLK_ESCAPE)
					{
						running = false;
					}
					else if (event.key.keysym.sym == SDLK_w)
					{
						buttons[Buttons::PaddleOneUp] = true;
					}
					else if (event.key.keysym.sym == SDLK_s)
					{
						buttons[Buttons::PaddleOneDown] = true;
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						buttons[Buttons::PaddleTwoUp] = true;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						buttons[Buttons::PaddleTwoDown] = true;
					}
				}
				else if (event.type == SDL_KEYUP)
				{
					if (event.key.keysym.sym == SDLK_w)
					{
						buttons[Buttons::PaddleOneUp] = false;
					}
					else if (event.key.keysym.sym == SDLK_s)
					{
						buttons[Buttons::PaddleOneDown] = false;
					}
					else if (event.key.keysym.sym == SDLK_UP)
					{
						buttons[Buttons::PaddleTwoUp] = false;
					}
					else if (event.key.keysym.sym == SDLK_DOWN)
					{
						buttons[Buttons::PaddleTwoDown] = false;
					}
				}
			}

			//Paddle processing
			if (buttons[Buttons::PaddleOneUp])
			{
				paddleOne.velocity.y = -PADDLE_SPEED;
			}
			else if (buttons[Buttons::PaddleOneDown])
			{
				paddleOne.velocity.y = PADDLE_SPEED;
			}
			else
			{
				paddleOne.velocity.y = 0.0f;
			}

			if (buttons[Buttons::PaddleTwoUp])
			{
				paddleTwo.velocity.y = -PADDLE_SPEED;
			}
			else if (buttons[Buttons::PaddleTwoDown])
			{
				paddleTwo.velocity.y = PADDLE_SPEED;
			}
			else
			{
				paddleTwo.velocity.y = 0.0f;
			}


			//Update the paddle positions
			paddleOne.Update(dt);
			if (!bot) 
			{
				paddleTwo.Update(dt);
			}
			else
			{
				//std::cout << static_cast<int>(bot_delay.front()) << std::endl;
				paddleTwo.AUpdate(dt, static_cast<int>(paddleTwo.bot_delay.front()));
				paddleTwo.bot_delay.pop();
				paddleTwo.bot_delay.push(ball.position.y);
			}
			
			//Update the ball position
			ball.Update(dt);

			//Check collisions
			Contact contact = CheckPaddleCollision(ball, paddleOne);
			if (contact.type != CollisionType::None)
			{
				ball.CollideWithPaddle(contact, dt);
				Mix_PlayChannel(-1, HitSound, 0);
			}
			else {
				contact = CheckPaddleCollision(ball, paddleTwo);
				if (contact.type != CollisionType::None)
				{
					ball.CollideWithPaddle(contact, dt);
					Mix_PlayChannel(-1, HitSound, 0);
				}
				else
				{
					contact = CheckWallCollision(ball);
					if (contact.type != CollisionType::None)
					{
						ball.CollideWithWall(contact);

						//Player one lose 
						if (contact.type == CollisionType::Left) 
						{
							++playerTwoScore;
							playerTwoScoreText.SetScore(playerTwoScore);
						}
						else
						{	
							//Player two lose 
							if (contact.type == CollisionType::Right)
							{
								++playerOneScore;
								playerOneScoreText.SetScore(playerOneScore);
							}
							else
							{
								Mix_PlayChannel(-1, HitSound, 0);
							}
						}
					}
				}
			}

			//Clear the window to black
			SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
			SDL_RenderClear(renderer);

			// Draw the net
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			for (int y = 0; y < WINDOW_HEIGHT; ++y)
			{
				if (y % 5)
				{
					SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, y);
				}
			}
			
			//Draw the ball
			ball.Draw(renderer);

			//Draw the paddles
			paddleOne.Draw(renderer);
			paddleTwo.Draw(renderer);
			//paddleTwo.Draw(renderer);

			//Display the scores
			playerOneScoreText.Draw();
			playerTwoScoreText.Draw();

			//Present the backbuffer
			SDL_RenderPresent(renderer);

			//Calculate frame time
			auto stopTime = std::chrono::high_resolution_clock::now();
			dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();
			//dt = dt * 1.5;  //faster 
			//std::cout << dt << std::endl;
		}
	}

	Mix_FreeChunk(HitSound);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	TTF_CloseFont(scoreFont);
	Mix_Quit();
	TTF_Quit();
	SDL_Quit();
	
	return 0;
}


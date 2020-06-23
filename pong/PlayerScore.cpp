#include "PlayerScore.h"


void PlayerScore::SetScore(int score)
{
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);

	surface = TTF_RenderText_Solid(font, std::to_string(score).c_str(), { 0xFF, 0xFF, 0xFF, 0xFF });
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	int width, height;
	SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
	rect.w = width;
	rect.h = height;
}

void PlayerScore::Draw()
{
	SDL_RenderCopy(renderer, texture, nullptr, &rect);
}
#pragma once
#include "State.h"
#include "Ball.h"
#include "Brick.h"
#include "ScoreSystem.h"
#include "Player.h"
#include "GameObject.h"

#define WIDTH 1280
#define HEIGHT 720

class InGameState : public State
{
private:

public:
	sf::Font* font;
	// 일단 필요하지 않을 것 같아 주석 처리
	// sf::Texture bgTexture;
	// sf::Sprite bgSprite;

	Ball* ball;
	ScoreSystem* scrSystem;
	std::vector<Brick*> bricks;
	std::pair<Player*, Player*> players;

	int winner_num = -1;

	InGameState(sf::RenderWindow* window);
	virtual ~InGameState();

	virtual void CheckForQuit();

	int EndState();
	void UpdateInput(const float& dt);
	void Update(const float& dt);
	void Render(sf::RenderTarget* target);
};



#pragma once

#include "GameState.h"
#include "SFML\Graphics\Text.hpp"
#include "SFML\Graphics\Sprite.hpp"

class MenuState : public GameState
{
public:
	MenuState();
	~MenuState();

	void entering();
	void leaving();
	void obscuring();
	void releaving();
	bool update(float dt);
	void draw();
	void setupActions();

private:
	
	sf::Sprite mLogo;
	sf::Text mText;

};


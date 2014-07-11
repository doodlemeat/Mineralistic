#pragma once

#include "GameState.h"
#include "SFML\Graphics\Text.hpp"

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

	sf::Text mText;

};


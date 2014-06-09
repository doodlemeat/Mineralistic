#pragma once
#include "GameState.h"

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
};


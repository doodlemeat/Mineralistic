#pragma once
#include "GameState.h"

class IntroState : public GameState
{
public:
	IntroState();
	~IntroState();

	void entering();
	void leaving();
	void obscuring();
	void releaving();
	bool update(float dt);
	void draw();
	void setupActions();
};


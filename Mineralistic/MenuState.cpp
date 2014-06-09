#include "MenuState.h"
#include "PlayState.h"
#include "GameStateAsset.h"
#include "GameEngine.h"
#include <iostream>

MenuState::MenuState()
{
}

MenuState::~MenuState()
{

}

void MenuState::entering()
{
	std::cout << "Entering menu state" << std::endl;
}

void MenuState::leaving()
{
	std::cout << "Leaving menu state" << std::endl;
}

void MenuState::obscuring()
{
}

void MenuState::releaving()
{
}

bool MenuState::update(float dt)
{
	mAssets->gameEngine->changeState(new PlayState());
	return true;
}

void MenuState::draw()
{

}

void MenuState::setupActions()
{

}

#include "IntroState.h"
#include "MenuState.h"
#include "GameEngine.h"
#include "GameStateAsset.h"

#include <iostream>
#include <Thor/Input/Action.hpp>
#include <Thor/Input/ActionMap.hpp>

IntroState::IntroState()
{
}

IntroState::~IntroState()
{

}

void IntroState::entering()
{
	std::cout << "Entering Intro state" << std::endl;
}

void IntroState::leaving()
{
	std::cout << "Leaving Intro state" << std::endl;
}

void IntroState::obscuring()
{
}

void IntroState::releaving()
{
}

bool IntroState::update(float dt)
{
	mAssets->gameEngine->changeState(new MenuState());
	return true;
}

void IntroState::draw()
{
	
}

void IntroState::setupActions()
{
	mActionMap->operator[]("Pause") = thor::Action(sf::Keyboard::Return, thor::Action::PressOnce);
}

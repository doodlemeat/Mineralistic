#include "MenuState.h"
#include "PlayState.h"
#include "GameStateAsset.h"
#include "GameEngine.h"
#include <iostream>
#include "AudioSystem.h"
#include "GameState.h"
#include "ResourceHolder.h"
#include "WindowManager.h"
#include "ObjectManager.h"
#include "SFML/Graphics/RenderWindow.hpp"

MenuState::MenuState()
{
}

MenuState::~MenuState()
{

}

void MenuState::entering()
{
	mAssets->audioSystem->playMusic("Ambient_1");
	std::cout << "Entering menu state" << std::endl;

	// Main text that will draw all the items
	mText.setFont(mAssets->resourceHolder->getFont("consola.ttf"));
	mText.setString("Main Menu");
	mText.setPosition(12, 12);
	mText.setCharacterSize(40);
	mText.setColor(sf::Color::Red);

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
	if (getActionMap()->isActive("Start"))
	{
		mAssets->gameEngine->changeState(new PlayState());
		return true;
	}
	if (getActionMap()->isActive("Quit"))
	{
		return false;
	}
	return true;
}

void MenuState::draw()
{
	mAssets->windowManager->getWindow()->setView(mAssets->windowManager->getWindow()->getDefaultView());
	
	mAssets->windowManager->getWindow()->draw(mText);
}

void MenuState::setupActions()
{
	getActionMap()->operator[]("Start") = thor::Action(sf::Keyboard::Return, thor::Action::PressOnce);
	getActionMap()->operator[]("Quit") = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce);
}

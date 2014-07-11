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

MenuState::MenuState() :
mLogo(),
mMenu()
{

}

MenuState::~MenuState()
{

}

void MenuState::entering()
{
	mAssets->audioSystem->playMusic("Ambient_1");
	std::cout << "Entering menu state" << std::endl;

	// These will be used to center elements
	// relative to the game Window
	auto windowSize = mAssets->windowManager->getWindow()->getSize();
	sf::FloatRect rect;
	
	// Awesome game logo!
	mLogo.setTexture(mAssets->resourceHolder->getTexture("logo.png"));

	// Centering and moving a little up
	rect = mLogo.getLocalBounds();
	mLogo.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
	mLogo.setPosition(sf::Vector2f(windowSize.x / 2.0f, (windowSize.y / 2.0f) - rect.height));

	mMenu.setFont(mAssets->resourceHolder->getFont("loaded.ttf"));
	mMenu.setPosition(300, 300);
	mMenu.addItem("Start Game", 0);
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
	mMenu.update(dt);

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
	mAssets->windowManager->getWindow()->draw(mLogo);
	mAssets->windowManager->getWindow()->draw(mMenu);
}

void MenuState::setupActions()
{
	getActionMap()->operator[]("Start") = thor::Action(sf::Keyboard::Return, thor::Action::PressOnce);
	getActionMap()->operator[]("Quit") = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce);
}

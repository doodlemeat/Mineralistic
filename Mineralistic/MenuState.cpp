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

// This is a local enum simply to make adding
// items to the Menu easier to read
enum MenuItemIDs 
{
	MENU_ITEM_START=0,
	MENU_ITEM_OPTIONS,
	MENU_ITEM_HELP,
	MENU_ITEM_EXIT
};

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

	auto windowSize = mAssets->windowManager->getWindow()->getSize();
	
	// Awesome game logo!
	mLogo.setTexture(mAssets->resourceHolder->getTexture("logo.png"));

	// Centering and moving a little up
	sf::FloatRect rect = mLogo.getLocalBounds();
	mLogo.setOrigin(rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f);
	mLogo.setPosition(sf::Vector2f(windowSize.x / 2.0f, (windowSize.y / 2.0f) - rect.height));

	mMenu.setFont(mAssets->resourceHolder->getFont("loaded.ttf"));
	mMenu.setPosition(windowSize.x/2.0f, windowSize.y/2.0f);
	mMenu.addItem("Start Game", MENU_ITEM_START);
	mMenu.addItem("Options", MENU_ITEM_OPTIONS);
	mMenu.addItem("Help", MENU_ITEM_HELP);
	mMenu.addItem("Quit", MENU_ITEM_EXIT);
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
	mMenu.update(dt, getActionMap());

	if (getActionMap()->isActive("Menu_Select"))
	{
		switch (mMenu.getCurrentID())
		{
		case MENU_ITEM_START:
			mAssets->gameEngine->changeState(new PlayState());
			break;
		case MENU_ITEM_OPTIONS:
			break;
		case MENU_ITEM_HELP:
			break;
		case MENU_ITEM_EXIT:
			return false;
			break;
		default:
			break;
		}
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
	getActionMap()->operator[]("Menu_Select") = thor::Action(sf::Keyboard::Return, thor::Action::PressOnce);
	getActionMap()->operator[]("Menu_Next") = thor::Action(sf::Keyboard::Down, thor::Action::PressOnce);
	getActionMap()->operator[]("Menu_Previous") = thor::Action(sf::Keyboard::Up, thor::Action::PressOnce);
	getActionMap()->operator[]("Quit") = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce);
}

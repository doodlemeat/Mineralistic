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
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML\Graphics\CircleShape.hpp>
#include <SFML\Graphics\RectangleShape.hpp>

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
mButtonList()
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

	mButtonList.setFont(mAssets->resourceHolder->getFont("loaded.ttf"));
	mButtonList.setPosition(windowSize.x/2.0f, windowSize.y/2.0f);
	mButtonList.setAssets(mAssets);
	mButtonList.addItem("Start Game", MENU_ITEM_START);
	mButtonList.addItem("Options", MENU_ITEM_OPTIONS);
	mButtonList.addItem("Help", MENU_ITEM_HELP);
	mButtonList.addItem("Quit", MENU_ITEM_EXIT);
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
	if (mPopUpHelp.isActive())
	{
		if (getActionMap()->isActive("ButtonList_Select") ||
		   (getActionMap()->isActive("Quit")))
		{
			mPopUpHelp.deactivate();
		}
		return true;
	}

	mButtonList.update(dt, getActionMap());

	if (getActionMap()->isActive("ButtonList_Select"))
	{
		switch (mButtonList.getCurrentID())
		{
		case MENU_ITEM_START:
			mAssets->gameEngine->changeState(new PlayState());
			break;
		case MENU_ITEM_OPTIONS:
			break;
		case MENU_ITEM_HELP:
			mPopUpHelp.activate();
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
	mAssets->windowManager->getWindow()->draw(mButtonList);

	// Help pop up must always be drawn last
	if (mPopUpHelp.isActive())
	{
		// Drawing the whole Help screen
		static sf::RectangleShape bg;
		if (bg.getSize().x == 0)
		{
			// Initializing background for the first time
			bg.setSize(sf::Vector2f(430, 300));
			bg.setPosition(sf::Vector2f(mButtonList.getPosition().x - bg.getSize().x, mButtonList.getPosition().y - 50));
			
			bg.setFillColor(sf::Color(255, 255, 255, 100));
			bg.setOutlineColor(sf::Color(255, 255, 255, 200));
		}

		static sf::Text text;
		if (text.getString() == "")
		{
			// Initializing text for the first time
			text.setFont(mAssets->resourceHolder->getFont("loaded.ttf"));
			text.setCharacterSize(30);
			text.setColor(sf::Color::White);
			text.setPosition(bg.getPosition().x + 5, bg.getPosition().y);

			std::string output = "";
			// This looks alright in-game
			// (even if it looks broken here)
			output += "Arrow keys         walk\n";
			output += "c                       dig\n";
			output += "z              put torch\n";
			output += "x             throw rope\n";
			output += "F1          debug screen\n";
			output += "ESC                   quit\n";

			text.setString(output);
		}

		mAssets->windowManager->getWindow()->draw(text);
		mAssets->windowManager->getWindow()->draw(bg);
	}
}

void MenuState::setupActions()
{
	getActionMap()->operator[]("ButtonList_Select") = thor::Action(sf::Keyboard::Return, thor::Action::PressOnce);
	getActionMap()->operator[]("ButtonList_Next") = thor::Action(sf::Keyboard::Down, thor::Action::PressOnce);
	getActionMap()->operator[]("ButtonList_Previous") = thor::Action(sf::Keyboard::Up, thor::Action::PressOnce);
	getActionMap()->operator[]("ButtonList_First") = thor::Action(sf::Keyboard::PageUp, thor::Action::PressOnce) || thor::Action(sf::Keyboard::Home, thor::Action::PressOnce);
	getActionMap()->operator[]("ButtonList_Last") = thor::Action(sf::Keyboard::PageDown, thor::Action::PressOnce) || thor::Action(sf::Keyboard::End, thor::Action::PressOnce);
	getActionMap()->operator[]("Quit") = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce);
}

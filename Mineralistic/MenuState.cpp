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
mText()
{

}

MenuState::~MenuState()
{

}

void MenuState::entering()
{
	mAssets->audioSystem->playMusic("Ambient_1");
	std::cout << "Entering menu state" << std::endl;

	mText.setFont(mAssets->resourceHolder->getFont("loaded.ttf"));	
	mText.setString("Press <enter> to Start");
	mText.setCharacterSize(40);
	mText.setColor(sf::Color::Red);

	// Centering it on the game Window
	sf::FloatRect textRect = mText.getLocalBounds();
	mText.setOrigin(textRect.left + textRect.width  / 2.0f,
	                textRect.top  + textRect.height / 2.0f);

	auto size = mAssets->windowManager->getWindow()->getSize();
	mText.setPosition(sf::Vector2f(size.x/2.0f, size.y/2.0f));
	
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
	mAssets->windowManager->getWindow()->draw(mText);
}

void MenuState::setupActions()
{
	getActionMap()->operator[]("Start") = thor::Action(sf::Keyboard::Return, thor::Action::PressOnce);
	getActionMap()->operator[]("Quit") = thor::Action(sf::Keyboard::Escape, thor::Action::PressOnce);
}

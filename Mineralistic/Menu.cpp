#include "Menu.h"
#include <SFML\Graphics\RenderTarget.hpp>

Menu::Menu() :
mFont(),
mPos(0, 0),
mCurrentItem(nullptr)
{

}

Menu::Menu(sf::Font font, float x = 0, float y = 0) :
mFont(font),
mPos(x, y),
mCurrentItem(nullptr)
{
	setFont(font);
	setPosition(x, y);
}

Menu::~Menu()
{

}

void Menu::setFont(sf::Font font)
{
	mFont = font;
}

void Menu::setPosition(float x, float y)
{
	mPos = sf::Vector2f(x, y);
}

void Menu::addItem(std::string text, int id)
{
	MenuItem newItem;
	newItem.mID = id;

	newItem.mText.setFont(mFont);
	newItem.mText.setString(text);
	newItem.mText.setPosition(mPos);
	newItem.mText.setColor(sf::Color::Red);
	newItem.mText.setCharacterSize(40);

	mItem.push_back(newItem);
}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto item : mItem)
	{
		target.draw(item.mText);
	}
}

void Menu::update(float dt)
{

}

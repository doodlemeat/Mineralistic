#include "Menu.h"
#include <SFML\Graphics\RenderTarget.hpp>

Menu::Menu() :
mFont(),
mPos(0, 0),
mCurrentIndex(-1)
{

}

Menu::Menu(sf::Font font, float x = 0, float y = 0) :
mFont(font),
mPos(x, y),
mCurrentIndex(-1)
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
	newItem.mText.setColor(sf::Color::White);
	newItem.mText.setCharacterSize(40);

	// Will pull down this item depending on how much
	// items this Menu already have
	sf::Vector2f newPosition;
	newPosition.x = mPos.x;
	newPosition.y = mPos.y + (mItem.size() * newItem.mText.getLocalBounds().height);
	newItem.mText.setPosition(newPosition);

	mItem.push_back(newItem);

	// Setting colors on the items if it's highlighted
	if (mCurrentIndex == -1)
	{
		mCurrentIndex = 0;
		mItem.at(mCurrentIndex).mText.setColor(sf::Color(69, 130, 196));
	}
}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto item : mItem)
	{
		target.draw(item.mText);
	}
}

void Menu::update(float dt, thor::ActionMap<std::string> *pActionMap)
{
	if (pActionMap->isActive("Menu_Next"))
	{
		next();
	}
	if (pActionMap->isActive("Menu_Previous"))
	{
		previous();
	}
}

void Menu::next()
{
	if (mCurrentIndex >= mItem.size())
	{
		return;
	}

	mItem.at(mCurrentIndex).mText.setColor(sf::Color::White);
	mCurrentIndex++;
	mItem.at(mCurrentIndex).mText.setColor(sf::Color(69, 130, 196));
}
void Menu::previous()
{
	if (mCurrentIndex <= 0)
	{
		return;
	}

	mItem.at(mCurrentIndex).mText.setColor(sf::Color::White);
	mCurrentIndex--;
	mItem.at(mCurrentIndex).mText.setColor(sf::Color(69, 130, 196));
}

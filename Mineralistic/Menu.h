#pragma once

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Text.hpp>
#include <Thor\Input\ActionMap.hpp>
#include <string>
#include <vector>

/// Container for individual menu item's stuff.
///
/// You should never mess with this. The *Menu* class handles
/// it all internally.
struct MenuItem
{
	sf::Text mText;
	int mID;
};

class Menu : public sf::Drawable
{
public:
	Menu();
	~Menu();

	void setFont(sf::Font font);
	void setPosition(float x, float y);

	/// Inserts an item with specified text.
	///
	/// @param id Unique identifier for this item.
	void addItem(std::string text, int id);

	/// Returns the custom value of the current item.
	///
	/// @return -1 if no items are set.
	int getCurrentValue();

	void update(float dt, thor::ActionMap<std::string> *pActionMap);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void next();
	void previous();
	void first();
	void last();

private:

	sf::Font mFont;
	sf::Vector2f mPos;

	std::vector<MenuItem> mItem;
	int mCurrentIndex;

	sf::Color mColorItem;
	sf::Color mColorItemHilite;
};


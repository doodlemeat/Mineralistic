#pragma once

#include <SFML\Graphics\Drawable.hpp>
#include <SFML\Graphics\Text.hpp>
#include <Thor\Input\ActionMap.hpp>
#include <string>
#include <vector>
#include "GameStateAsset.h"

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

	/// Returns the identifier of the current item.
	///
	/// @return -1 if no items are set.
	int getCurrentID();

	void update(float dt, thor::ActionMap<std::string> *pActionMap);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void next();
	void previous();
	void first();
	void last();

	/// We only need this so we can play sounds
	/// @todo Find better way of doing this
	///       (should the Menu be a GameObject?)
	void setAssets(GameStateAsset* mAsset);

private:
	void changeCurrentIndex(int newValue);

	sf::Font mFont;
	sf::Vector2f mPos;

	std::vector<MenuItem> mItem;
	int mCurrentIndex;

	sf::Color mColorItem;
	sf::Color mColorItemHilite;

	GameStateAsset* mAssets;
};


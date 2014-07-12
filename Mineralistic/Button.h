#pragma once

#include <SFML\Graphics\Text.hpp>

/// Container for individual ButtonList item's stuff.
///
/// You should never mess with this. The *ButtonList* class handles
/// it all internally.
class Button
{
public:
	sf::Text mText;
	int mID;
};


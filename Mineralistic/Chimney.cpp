#include "Chimney.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include <iostream>


Chimney::Chimney()
{
	mActive = false;
}


Chimney::~Chimney()
{
}

void Chimney::update(float dt, thor::ActionMap<std::string> *pActionMap)
{

}

void Chimney::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*mSprite);
}

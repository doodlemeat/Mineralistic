#include "Hook.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include <iostream>


Hook::Hook()
{
}


Hook::~Hook()
{
}

void Hook::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*mSprite);
}

void Hook::update(float dt)
{

}

#include "House.h"
#include "SFML\Graphics\RenderTarget.hpp"


House::House()
{
	mActive = false;
}


House::~House()
{
}

void House::update(float dt, thor::ActionMap<std::string> *pActionMap)
{

}

void House::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*mSprite);
}

void House::setChimney(Chimney *pChimney)
{
	mChimney = pChimney;
}

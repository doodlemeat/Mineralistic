#include "Torch.h"
#include "SFML\Graphics\RenderTarget.hpp"


Torch::Torch()
{
}


Torch::~Torch()
{
}

void Torch::update(float dt)
{

}

void Torch::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*mSprite);
}

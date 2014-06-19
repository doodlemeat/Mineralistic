#pragma once

#include "GameObject.h"

class Torch: public GameObject
{
public:
	Torch();
	~Torch();

	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;
};


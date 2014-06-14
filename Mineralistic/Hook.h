#pragma once

#include "GameObject.h"

class Hook: public GameObject
{
public:
	Hook();
	~Hook(); 
	
	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

};


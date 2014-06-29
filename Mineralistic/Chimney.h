#pragma once
#include "GameObject.h"

class Chimney : public GameObject
{
public:
	Chimney();
	~Chimney();

	void update(float dt, thor::ActionMap<std::string> *pActionMap);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

private:
	bool mActive;
};


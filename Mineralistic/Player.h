#pragma once

#include "GameObject.h"
#include "SFML\Graphics\View.hpp"

class World;

class Player : public GameObject
{
public:
	Player();
	~Player();

	virtual void update(float dt);
	virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	sf::View *getView();
	void centerOrigin();

	void setWorld(World *pWorld);
	World *getWorld();

private:
	sf::View* mView;

	World *mWorld;
};


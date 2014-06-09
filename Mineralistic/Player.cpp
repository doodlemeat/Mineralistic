#include "Player.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include "GameObject.h"


Player::Player() : GameObject("Player")
{
	mView = new sf::View();
}


Player::~Player()
{
	delete mView;
	mView = nullptr;
}

void Player::update(float dt)
{
	getAnimator()->update(sf::seconds(dt));
	getAnimator()->animate(*mSprite);

	mView->setCenter(mSprite->getPosition());
}

void Player::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*mSprite);
}

sf::View * Player::getView()
{
	return mView;
}

void Player::setWorld(World *pWorld)
{
	mWorld = pWorld;
}

World * Player::getWorld()
{
	return mWorld;
}

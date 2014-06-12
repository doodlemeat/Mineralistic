#include "Player.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include "GameObject.h"
#include "Box2D/Box2D.h"
#include "Thor/Input/ActionMap.hpp"
#include "PhysicsScale.h"
#include "World.h"
#include <iostream>

Player::Player() : GameObject("Player")
{
	mView = new sf::View();
	mFootContacts = 0;
	mRopes = 5;
	mStopWatch.restart();
}


Player::~Player()
{
	delete mView;
	mView = nullptr;
	
	delete mB2UserData;
	mB2UserData = nullptr;
}

void Player::update(float dt, thor::ActionMap<std::string> *pActionMap)
{
	b2Vec2 velocity = mBody->GetLinearVelocity();
	mBody->SetLinearVelocity(b2Vec2(0, velocity.y));

	if (pActionMap->isActive("Jump") && canJump() && mStopWatch.getElapsedTime().asMilliseconds() > 100)
	{
		mStopWatch.restart();
		mBody->ApplyLinearImpulse(PhysicsScale::gameToPhys(sf::Vector2f(0, -150)), mBody->GetWorldCenter(), true);
	}
	if (pActionMap->isActive("Walk_Left"))
	{
		mBody->ApplyLinearImpulse(PhysicsScale::gameToPhys(sf::Vector2f(-50, 0)), mBody->GetWorldCenter(), true);
	}
	if (pActionMap->isActive("Walk_Right"))
	{
		mBody->ApplyLinearImpulse(PhysicsScale::gameToPhys(sf::Vector2f(50, 0)), mBody->GetWorldCenter(), true);
	}

	if (pActionMap->isActive("Throw_Rope"))
	{
		if (mRopes > 0)
		{
			try
			{
				Tile *targetTile = mWorld->getClosestTileInDirection(WorldHelper::toWorldPositionFromSFMLPosition(mSprite->getPosition()), World::NORTH, 10);
			}
			catch (WorldException &e)
			{
				std::cout << e.what() << std::endl;
			}
		}
	}


	sf::Vector2f newPosition = PhysicsScale::physToGame(mBody->GetPosition()) - sf::Vector2f(0, 4);
	mSprite->setPosition(newPosition);

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

void Player::setB2UserData(B2UserData *pUserData)
{
	mB2UserData = pUserData;
}

void Player::increaseFootContacts()
{
	mFootContacts++;
}

void Player::decreaseFootContacts()
{
	mFootContacts--;
}

bool Player::canJump()
{
	return mFootContacts > 0;
}

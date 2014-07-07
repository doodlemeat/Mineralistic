#include "Monster.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include "PhysicsScale.h"
#include "GameObject.h"
#include "Box2D/Box2D.h"
#include "ObjectManager.h"
#include "World.h"
#include <iostream>
#include "Logger.h"
#include "Pathfinder.h"
#include "SFML/Graphics/CircleShape.hpp"
#include "Node.h"
#include "Tile.h"
#include <vector>


Monster::Monster()
{
	mState = IDLING;
}

Monster::~Monster()
{
	delete mPathfinder;
}

void Monster::update(float dt)
{
	switch (mState)
	{
	case IDLING:
	{
		sf::Vector2f SFMLPosPlayer = mObjectManager->getObject("Player")->getSprite()->getPosition();
		float worldDistance = WorldHelper::worldDistanceBetweenSFMLPos(SFMLPosPlayer, mSprite->getPosition());

		if (worldDistance <= 10)
		{
			mState = CHASING;
		}
		break;
	}
	case CHASING:
	{	
		mPathfinder->computeGrid(sf::Vector2i(WorldHelper::toWorldPositionFromSFMLPosition(mSprite->getPosition())),
		sf::Vector2i(WorldHelper::toWorldPositionFromSFMLPosition(mObjectManager->getObject("Player")->getSprite()->getPosition())));
		mPathfinder->search();
		sf::Vector2f SFMLPosPlayer = mObjectManager->getObject("Player")->getSprite()->getPosition();
		float distanceToPlayer = WorldHelper::worldDistanceBetweenSFMLPos(SFMLPosPlayer, mSprite->getPosition());
		if (distanceToPlayer > 10)
		{
			mState = IDLING;
		}
		break;
	}
	case ATTACKING:
	{
		break;
	}
	}

	sf::Vector2f newPosition = PhysicsScale::physToGame(mBody->GetPosition()) - sf::Vector2f(0, 4);
	mSprite->setPosition(newPosition);

	getAnimator()->update(sf::seconds(dt));
	getAnimator()->animate(*mSprite);
}

void Monster::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (mState == CHASING)
	{
		std::vector<Node*> path = mPathfinder->getPath();
		for (int i = 0; i < path.size(); i++)
		{
			sf::CircleShape shape;
			shape.setRadius(20);
			shape.setOrigin(20, 20);
			shape.setFillColor(sf::Color::Green);
			sf::Vector2f worldPos = path[i]->worldRef->getPosition();
			sf::Vector2f SFMLPos = WorldHelper::toSFMLPositionFromWorldPosition(worldPos, true);
			shape.setPosition(SFMLPos);
			target.draw(shape);
		}
	}

	target.draw(*mSprite);
}

void Monster::changeState(MonsterState pNewState)
{
	mState = pNewState;
}

Pathfinder* Monster::getPathfinder()
{
	return mPathfinder;
}

void Monster::initPathfinder()
{
	mPathfinder = new Pathfinder(mObjectManager->getWorld(), 30, 30);
}

#include "Monster.h"
#include "SFML\Graphics\RenderTarget.hpp"
#include "PhysicsScale.h"
#include "GameObject.h"
#include "Box2D/Box2D.h"


Monster::Monster()
{
}


Monster::~Monster()
{
}

void Monster::update(float dt)
{
	switch (mState)
	{
	case IDLING:

		break;
	case CHASING:

		break;
	case ATTACKING:

		break;
	}

	sf::Vector2f newPosition = PhysicsScale::physToGame(mBody->GetPosition()) - sf::Vector2f(0, 4);
	mSprite->setPosition(newPosition);

	getAnimator()->update(sf::seconds(dt));
	getAnimator()->animate(*mSprite);
}

void Monster::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(*mSprite);
}

void Monster::changeState(MonsterState pNewState)
{
	mState = pNewState;
}

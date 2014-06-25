#pragma once

#include "GameObject.h"

class Monster : public GameObject
{
	enum MonsterState
	{
		IDLING,
		CHASING,
		ATTACKING
	};

public:
	Monster();
	~Monster();

	void update(float dt);
	void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	void changeState(MonsterState pNewState);
private:
	MonsterState mState;
};


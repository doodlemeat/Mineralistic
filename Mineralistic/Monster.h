#pragma once

#include "GameObject.h"

class Pathfinder;

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

	void initPathfinder();
	void changeState(MonsterState pNewState);

	Pathfinder* getPathfinder();
private:

	MonsterState mState;
	Pathfinder *mPathfinder;
};


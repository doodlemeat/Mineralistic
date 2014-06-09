#pragma once

#include "GameState.h"
#include <vector>

class b2World;
class ObjectManager;
class WindowManager;
class World;
class Box2DWorldDraw;

class PlayState : public GameState
{
public:
	PlayState();
	~PlayState();

	void entering();
	void leaving();
	void obscuring();
	void releaving();
	bool update(float dt);
	void draw();
	void setupActions();
	void generateChunk();

private:
	ObjectManager *mObjectManager;
	World *mWorld;
	b2World* mB2World;
	Box2DWorldDraw *mB2DebugDraw;
};


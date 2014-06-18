#pragma once

#include "GameState.h"
#include <vector>
#include "SFML\Graphics\Text.hpp"

class b2World;
class ObjectManager;
class WindowManager;
class World;
class Box2DWorldDraw;
class B2ContactListener;

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

	void setupPlayer();
	void registerMaterials();

private:
	bool mDebugToggled;

	sf::Text mDebug;

	ObjectManager *mObjectManager;
	World *mWorld;
	b2World* mB2World;
	B2ContactListener *mContactListener;
	Box2DWorldDraw *mB2DebugDraw;
};


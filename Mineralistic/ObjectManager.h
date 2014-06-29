#pragma once

#include <vector>
#include "Thor/Input/ActionMap.hpp"

class ObjectGroup;
class AudioSystem;
class GameObject;
class ResourceHolder;
class Hook;
class b2World;
class b2Body;
class Tile;
class b2Joint;
class World;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();
	
	bool hasGroup(std::string pName);
	bool hasObject(std::string pName);

	ObjectGroup *getGroup(std::string pName);
	GameObject *getObject(std::string pName);

	void setB2World(b2World *pWorld);
	void setAudioSystem(AudioSystem *pAudiosystem);
	void setResourceHolder(ResourceHolder *pResourceHolder);
	AudioSystem *getAudioSystem();

	void addObject(GameObject *pObject);
	void addGroup(ObjectGroup *pGroup);
	void update(float dt, thor::ActionMap<std::string> *pActionMap);
	std::vector<ObjectGroup*> getGroups();
	std::vector<GameObject*> getObjects();

	Hook *spawnHook(sf::Vector2f pWorldPosition);

	b2Joint *createDistanceJointBetween(b2Body *pBodyA, b2Body *pBodyB, float pLength);
	b2World *getB2World();
	World *getWorld();
	void setWorld(World *pWorld);
	void spawnMonster(sf::Vector2f pWorldPosition);
	void spawnTorch(sf::Vector2f pWorldPosition);
	void orderGameObjects();
private:
	std::vector<ObjectGroup*> mObjectGroups;
	std::vector<GameObject*> mGameObjects;
	AudioSystem *mAudioSystem;
	ResourceHolder *mResourceHolder;
	b2World *mB2World;
	World *mWorld;
};

